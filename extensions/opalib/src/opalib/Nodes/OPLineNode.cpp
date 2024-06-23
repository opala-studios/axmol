//
// Created by Pichau on 11/27/2020.
//

#include <cocos2d/cocos/renderer/CCTexture2D.h>
#include "OPLineNode.hpp"
using namespace opalib;

#ifdef COCOS2D_DEBUG
#define OP_LINENODE_DEBUG_TRIANGLES 1
#else
//always force 0 on release builds
#define OP_LINENODE_DEBUG_TRIANGLES 0
#endif



static const char* LINE_NODE_VERTEX_SHADER_CODE = R"(
attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
#endif

uniform float u_offsetX;

void main()
{
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
    v_texCoord.x += u_offsetX;
}
)";

static const char* LINE_NODE_TEXTURIZED_FRAGMENT_SHADER_CODE = R"(
#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
    gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
}
)";

static const char* LINE_NODE_FULL_COLOR_FRAGMENT_SHADER_CODE = R"(
#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
    gl_FragColor = v_fragmentColor;
}
)";



USING_NS_CC;

OPLineNode::~OPLineNode() {
	free(_buffer);
	_buffer = nullptr;

	glDeleteBuffers(1, &_vbo);
	_vbo = 0;

	if (Configuration::getInstance()->supportsShareableVAO()) {
		GL::bindVAO(0);
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}
}

bool OPLineNode::init(cocos2d::Texture2D* texture) {
	_blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;

//	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
#if OP_LINENODE_DEBUG_TRIANGLES
    setGLProgramState(GLProgramState::getOrCreateWithGLProgram(GLProgram::createWithByteArrays(LINE_NODE_VERTEX_SHADER_CODE, LINE_NODE_FULL_COLOR_FRAGMENT_SHADER_CODE)));
#else
    setGLProgramState(GLProgramState::getOrCreateWithGLProgram(GLProgram::createWithByteArrays(LINE_NODE_VERTEX_SHADER_CODE, LINE_NODE_TEXTURIZED_FRAGMENT_SHADER_CODE)));
#endif

	ensureCapacity(512);
	setupBuffer();

	setTexture(texture);
    updateTextureParameters(texture);
    
	_dirty = true;
	return true;
}

void OPLineNode::ensureCapacity(unsigned int count) {

	if(_bufferCount + count > _bufferCapacity) {
		_bufferCapacity += MAX(_bufferCapacity, count);
		_buffer = (V2F_C4B_T2F*)realloc(_buffer, _bufferCapacity*sizeof(V2F_C4B_T2F));
	}
}

void OPLineNode::setupBuffer() {
	if (Configuration::getInstance()->supportsShareableVAO()) {
		glGenVertexArrays(1, &_vao);
		GL::bindVAO(_vao);
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
		// vertex
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
		// color
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
		// texcoord
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));

		GL::bindVAO(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else {
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	CHECK_GL_ERROR_DEBUG();
}

void OPLineNode::drawTexturedCardinalSpline(cocos2d::PointArray *config, float tension,
											uint32_t segments,
											float radius, const cocos2d::Color4F &color) {
	Vec2 *vertices = calculateVertices(config, tension, segments);

	float lineLength = 0;
	Vec2 lastSegmentNormal = Vec2(0, 0);
	for (int i = 2; i <= segments + 1; ++i) {
        drawTexturizedSegment(vertices[i - 2], vertices[i - 1], radius, color, lineLength,
                              lastSegmentNormal);
	}

	CC_SAFE_DELETE_ARRAY(vertices);
}

Vec2 *OPLineNode::calculateVertices(PointArray *config, float tension, unsigned int segments) {
	auto *vertices = new Vec2[segments + 1];
	// copied from DrawNode cardinal splines with fix for border
	ssize_t p;
	float lt;
	float deltaT = 1.0f / (config->count() - 1);

	for (unsigned int i = 0; i < segments + 1; i++) {
		float dt = (float) i / segments;
		// border
		if (dt == 1) {
			p = config->count() - 1;
			lt = 1;
		} else {
			p = (ssize_t) (dt / deltaT);
			lt = (dt - deltaT * (float) p) / deltaT;
		}
		// Interpolate
		const Vec2 &pp0 = config->getControlPointAtIndex(p - 1);
		const Vec2 &pp1 = config->getControlPointAtIndex(p + 0);
		const Vec2 &pp2 = config->getControlPointAtIndex(p + 1);
		const Vec2 &pp3 = config->getControlPointAtIndex(p + 2);

		Vec2 newPos = ccCardinalSplineAt(pp0, pp1, pp2, pp3, tension, lt);
		vertices[i].x = newPos.x;
		vertices[i].y = newPos.y;
	}

	return vertices;
}

void OPLineNode::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {
	if(_bufferCount) {
		_customCommand.init(_globalZOrder, transform, flags);
		_customCommand.func = CC_CALLBACK_0(OPLineNode::onDraw, this, transform, flags);
		renderer->addCommand(&_customCommand);
	}
}

void OPLineNode::onDraw(const Mat4 &transform, uint32_t flags) {
	getGLProgramState()->apply(transform);
	auto glProgram = this->getGLProgram();
    _cumulatedOffSet +=  Director::getInstance()->getDeltaTime();
    glProgram->setUniformLocationWith1f(glProgram->getUniformLocation("u_offsetX"), _cumulatedOffSet*_speed);
	GL::bindTexture2D(_textureID);
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);

    if (_dirty) {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacity, _buffer, GL_STREAM_DRAW);

        _dirty = false;
    }
    if (Configuration::getInstance()->supportsShareableVAO()) {
        GL::bindVAO(_vao);
    }
    else {
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        // vertex
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // texcoord
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
    }

    glDrawArrays(GL_TRIANGLES, 0, _bufferCount);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (Configuration::getInstance()->supportsShareableVAO()) {
        GL::bindVAO(0);
    }

    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _bufferCount);
    CHECK_GL_ERROR_DEBUG();
}

void OPLineNode::setTexture(cocos2d::Texture2D *texture) {
	if (texture){
		_textureID = texture->getName();
	}
	else {
		_textureID = 0;
	}
    updateTextureParameters(texture);
}

void OPLineNode::drawTexturizedSegment(const Vec2 &from, const Vec2 &to, float radius,
                                       const Color4F &color, float &accumulatedLineLength,
                                       Vec2& lastSegmentNormal) {
    bool hasConnection = !lastSegmentNormal.isZero();
	unsigned int vertex_count = (hasConnection ? 4 : 2) * 3;
	ensureCapacity(vertex_count);

	Vec2 a = from;
	Vec2 b = to;

	Vec2 axis = b - a;
	float length = axis.length() * 0.01f;//TODO - dividir pelo tamanho da textura

	Vec2 n = axis.getPerp().getNormalized();
	Vec2 t = n.getPerp();

	Vec2 nw = n * radius;
	Vec2 tw = t * radius;
	Vec2 v0 = b - nw;
	Vec2 v1 = b + nw;
	Vec2 v2 = a - nw;
	Vec2 v3 = a + nw;

	V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);

#if OP_LINENODE_DEBUG_TRIANGLES
    Color4B triangle0Color = Color4B::RED;
    Color4B triangle1Color = Color4B::BLUE;
    Color4B triangle2Color = Color4B::GREEN;
#else
    Color4B triangle0Color = Color4B::WHITE;
	Color4B triangle1Color = Color4B::WHITE;
	Color4B triangle2Color = Color4B::WHITE;
#endif


	V2F_C4B_T2F_Triangle triangle0 = {
			{v1, triangle0Color, Tex2F(accumulatedLineLength + length, 1)},
			{v2, triangle0Color, Tex2F(accumulatedLineLength, 0)},
			{v0, triangle0Color, Tex2F(accumulatedLineLength + length, 0)},
	};
	triangles[0] = triangle0;

	V2F_C4B_T2F_Triangle triangle1 = {
			{v1, triangle1Color, Tex2F(accumulatedLineLength + length, 1)},
			{v2, triangle1Color, Tex2F(accumulatedLineLength, 0)},
			{v3, triangle1Color, Tex2F(accumulatedLineLength, 1)},
	};
	triangles[1] = triangle1;

	if (hasConnection){

	    Vec2 lastSegmentV0 = a - lastSegmentNormal * radius;
	    Vec2 lastSegmentV1 = a + lastSegmentNormal * radius;

        V2F_C4B_T2F_Triangle connectionUp = {
                {a, Color4B(color), Tex2F(accumulatedLineLength, 0.5f)},
                {v3, Color4B(color), Tex2F(accumulatedLineLength, 1)},
                {lastSegmentV1, Color4B(color), Tex2F(accumulatedLineLength, 1)},
        };
        triangles[2] = connectionUp;

        V2F_C4B_T2F_Triangle connectionDown = {
                {a, triangle2Color, Tex2F(accumulatedLineLength, 0.5f)},
                {lastSegmentV0, triangle2Color, Tex2F(accumulatedLineLength, 0)},
                {v2, triangle2Color, Tex2F(accumulatedLineLength, 0)},
        };
        triangles[3] = connectionDown;

	}
    lastSegmentNormal = n;
    accumulatedLineLength += length;
	_bufferCount += vertex_count;
	_dirty = true;
}

void OPLineNode::clear() {
	_bufferCount = 0;
	_dirty = true;
}

void OPLineNode::updateTextureParameters(cocos2d::Texture2D *texture) {
    Texture2D::TexParams tRepeatParams;
    tRepeatParams.magFilter = GL_LINEAR;
    tRepeatParams.minFilter = GL_LINEAR;
    tRepeatParams.wrapS = GL_REPEAT;
    tRepeatParams.wrapT = GL_REPEAT;
    texture->setTexParameters(tRepeatParams);
}


void OPLineNode::drawAnimatedCardinalSpline(cocos2d::PointArray *config, float tension,
                                            uint32_t segments, float radius, const Color4F &color) {
	Vec2* vertices = calculateVertices(config, tension, segments);

	unsigned int validTriangleCount;
	auto triangles = std::move(calculateIntersectionTriangles(vertices, segments + 1, radius,
                                                    validTriangleCount));

    generateLineMesh(triangles, validTriangleCount);

    CC_SAFE_DELETE_ARRAY(vertices);
}

void OPLineNode::generateLineMesh(const std::vector<IntersectionTriangle> &intersections,
                                  unsigned int triangleIntersectionsCount) {

    unsigned int vertexCount = (intersections.size() - 1) * 6 + (triangleIntersectionsCount * 3);
    ensureCapacity(vertexCount);

    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);

    float accumulatedBottomLength = 0;
    float accumulatedUpperLength = 0;
    for (int i = 0; i < intersections.size() - 1; i++){
        IntersectionTriangle intersectionA = intersections[i];
        IntersectionTriangle intersectionB = intersections[i + 1];
        Vec2 v0;
        Vec2 v1;
        Vec2 v2;
        Vec2 v3;

        if (intersectionA.angle == 0){
            v0 = intersectionA.junctionA;
            v1 = intersectionA.junctionB;
        }
        else {
            if (intersectionA.angle > 0){
                v0 = intersectionA.intersectionPoint;
                v1 = intersectionA.junctionB;
            }
            else {
                v0 = intersectionA.junctionB;
                v1 = intersectionA.intersectionPoint;
            }
        }

        if (intersectionB.angle == 0){
            v2 = intersectionB.junctionA;
            v3 = intersectionB.junctionB;
        }
        else {
            if (intersectionB.angle > 0){
                v2 = intersectionB.intersectionPoint;
                v3 = intersectionB.junctionA;
            }
            else {
                v2 = intersectionB.junctionA;
                v3 = intersectionB.intersectionPoint;
            }
        }

        float length = (v0 - v2).length() / 64;

#if OP_LINENODE_DEBUG_TRIANGLES
        Color4B triangle0Color = Color4B::RED;
        Color4B triangle1Color = Color4B::BLUE;
        Color4B triangle2Color = Color4B::GREEN;
#else
        Color4B triangle0Color = Color4B::WHITE;
	    Color4B triangle1Color = Color4B::WHITE;
	    Color4B triangle2Color = Color4B::WHITE;
#endif

        V2F_C4B_T2F_Triangle triangle0 = {
                {v0, triangle0Color, Tex2F(accumulatedBottomLength, 0)},
                {v2, triangle0Color, Tex2F(accumulatedBottomLength + length, 0)},
                {v3, triangle0Color, Tex2F(accumulatedUpperLength + length, 1)},
        };
        triangles[0] = triangle0;

        V2F_C4B_T2F_Triangle triangle1 = {
                {v0, triangle1Color, Tex2F(accumulatedBottomLength, 0)},
                {v3, triangle1Color, Tex2F(accumulatedUpperLength + length, 1)},
                {v1, triangle1Color, Tex2F(accumulatedUpperLength, 1)},
        };
        triangles[1] = triangle1;

        accumulatedBottomLength += length;
        accumulatedUpperLength += length;

        triangles += 2;

        if (intersectionB.angle != 0){
            float intersectionLength = (intersectionB.junctionA - intersectionB.junctionB).length() / 64;
            Vec2 v4 = intersectionB.junctionB;
            float sideHeight;
            float* accumulatedSideLength;
            if (intersectionB.angle > 0){
                accumulatedSideLength = &accumulatedUpperLength;
                sideHeight = 1;
            }
            else {
                accumulatedSideLength = &accumulatedBottomLength;
                sideHeight = 0;
            }
            V2F_C4B_T2F_Triangle triangleIntersection = {
                    {v2, triangle2Color, Tex2F(accumulatedBottomLength, 0)},
                    {v4, triangle2Color, Tex2F(*accumulatedSideLength + intersectionLength, sideHeight)},
                    {v3, triangle2Color, Tex2F(accumulatedUpperLength, 1)},
            };
            triangles[0] = triangleIntersection;
            triangles++;
            *accumulatedSideLength += intersectionLength;
        }
    }
    _bufferCount += vertexCount;
    _dirty = true;
}

std::vector<OPLineNode::IntersectionTriangle>
OPLineNode::calculateIntersectionTriangles(const cocos2d::Vec2 *vertices, unsigned int vertexCount,
                                           float radius, unsigned int &validTriangleCount) {
    std::vector<IntersectionTriangle> triangles;
    triangles.reserve(vertexCount);

    Vec2 initialAxis = vertices[1] - vertices[0];
    Vec2 initialNormal = initialAxis.getPerp().getNormalized() * radius;

    validTriangleCount = 0;

    //first and last triangles are not valid
    IntersectionTriangle initialTriangle;
    initialTriangle.junctionA = vertices[0] - initialNormal;
    initialTriangle.junctionB = vertices[0] + initialNormal;

    triangles.emplace_back(initialTriangle);

    //the first and last vertices will not generate full triangles
    for (int i = 0; i < vertexCount - 2; i++)
    {
        Vec2 a = vertices[i];
        Vec2 b = vertices[i + 1];
        Vec2 c = vertices[i + 2];
        Vec2 ab = b - a;
        Vec2 bc = c - b;

        Vec2 abN = ab.getPerp().getNormalized();
        Vec2 bcN = bc.getPerp().getNormalized();

        Vec2 abNR = abN * radius;
        Vec2 bcNR = bcN * radius;

        float angle = ab.getAngle() - bc.getAngle();
        if (angle > M_PI){
            angle -= 2 * M_PI;
        }
        else if (angle < -M_PI){
            angle += 2 * M_PI;
        }
        IntersectionTriangle triangle;
        triangle.angle = angle;
        if (angle != 0) {

            float _b = sinf(angle) * radius;
            float _a = cosf(angle) * radius;
            float _v = (radius - _a) / tanf(angle);
            float _k = radius - _b;
            float _l = radius - _k - _v;
//            _l = std::min(_l, std::min(ab.length(), bc.length()));

            Vec2 abAxis = ab.getNormalized();
            if (angle < 0) {
                triangle.junctionA = b - abNR;
                triangle.junctionB = b - bcNR;
                triangle.intersectionPoint = (b + abNR) + (abAxis * _l);
            }
            else {
                triangle.junctionA = b + abNR;
                triangle.junctionB = b + bcNR;
                triangle.intersectionPoint = (b - abNR) - (abAxis * _l);
            }
            validTriangleCount++;
        }
        else {
            triangle.junctionA = b - abNR;
            triangle.junctionB = b + abNR;
        }
        triangles.emplace_back(triangle);
    }

    Vec2 finalAxis = vertices[vertexCount - 1] - vertices[vertexCount - 2];
    Vec2 finalNormal = finalAxis.getPerp().getNormalized() * radius;

    //first and last triangles are not valid
    IntersectionTriangle finalTriangle;
    finalTriangle.junctionA = vertices[vertexCount - 1] - finalNormal;
    finalTriangle.junctionB = vertices[vertexCount - 1] + finalNormal;
    triangles.emplace_back(finalTriangle);

    return triangles;
}

void OPLineNode::drawAnimatedLine(cocos2d::PointArray *config, float radius) {

    uint32_t validTriangleCount;
    auto triangles = std::move(calculateIntersectionTriangles(config->getControlPoints().data(), config->getControlPoints().size(), radius, validTriangleCount));

    generateLineMesh(triangles, validTriangleCount);
}
