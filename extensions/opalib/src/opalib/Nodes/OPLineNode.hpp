//
// Created by Pichau on 11/27/2020.
//

#ifndef LineDrawer_hpp
#define LineDrawer_hpp

#include <cocos2d.h>
#include <Misc/OPMacros.hpp>

namespace opalib {

	class OPLineNode : public cocos2d::Node {
	private:
	    struct IntersectionTriangle {
	        cocos2d::Vec2 junctionA, junctionB;
	        cocos2d::Vec2 intersectionPoint;
	        float angle = 0;
	    };
	public:

		CREATE_FUNC_WITH_ARGS(OPLineNode);
		virtual ~OPLineNode();

		bool init(cocos2d::Texture2D* texture);

		void setTexture(cocos2d::Texture2D* texture);

		void drawTexturedCardinalSpline(cocos2d::PointArray *config, float tension,
										uint32_t segments,
										float radius,
										const cocos2d::Color4F &color = cocos2d::Color4F::WHITE);

		void drawTexturizedSegment(const cocos2d::Vec2 &from, const cocos2d::Vec2 &to, float radius,
								   const cocos2d::Color4F &color, float &accumulatedLineLength,
								   cocos2d::Vec2& lastSegmentNormal);

		void drawAnimatedCardinalSpline(cocos2d::PointArray *config, float tension,
                                        uint32_t segments,
                                        float radius,
                                        const cocos2d::Color4F &color = cocos2d::Color4F::WHITE);

		void drawAnimatedLine(cocos2d::PointArray *config, float radius);

		void clear();
        
        void updateTextureParameters(cocos2d::Texture2D* texture);

    protected:

		void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
        void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
		cocos2d::Vec2 *calculateVertices(cocos2d::PointArray *config,
										 float tension,
										 unsigned int segments);

		std::vector<OPLineNode::IntersectionTriangle>
        calculateIntersectionTriangles(const cocos2d::Vec2 *vertices, unsigned int vertexCount,
                                       float radius, unsigned int &validTriangleCount);

        void generateLineMesh(const std::vector<IntersectionTriangle> &intersections,
                              unsigned int triangleIntersectionsCount);

		void ensureCapacity(unsigned int count);
		void setupBuffer();

		GLuint _vao = 0;
		GLuint _vbo = 0;
		int _bufferCapacity = 0;

		GLsizei _bufferCount = 0;
		cocos2d::V2F_C4B_T2F *_buffer = nullptr;
		cocos2d::BlendFunc   _blendFunc;

		cocos2d::CustomCommand _customCommand;
		bool _dirty = false;
        
        double _cumulatedOffSet = 0;
        double _speed = 5.0;

		GLuint _textureID = 0;
	};
}
#endif //LineDrawer_hpp
