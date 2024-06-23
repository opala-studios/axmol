//
// Created by Talles Borges  on 26/06/20.
//

#ifndef OPALIBTEMPLATE_OPALIB_OPALIB_MISC_OPMACROS_HPP_
#define OPALIBTEMPLATE_OPALIB_OPALIB_MISC_OPMACROS_HPP_

#define CREATE_FUNC_WITH_ARGS(__TYPE__)                                 \
    template <typename... Args> static __TYPE__* create(Args&&... args) \
    {                                                                   \
        __TYPE__* pRet = new (std::nothrow) __TYPE__();                 \
        if (pRet && pRet->init(std::forward<Args>(args)...)) {          \
            pRet->autorelease();                                        \
            return pRet;                                                \
        } else {                                                        \
            delete pRet;                                                \
            pRet = NULL;                                                \
            return NULL;                                                \
        }                                                               \
    }

#endif // OPALIBTEMPLATE_OPALIB_OPALIB_MISC_OPMACROS_HPP_
