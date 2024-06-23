#include "../../Backend.h"

#include <jni.h>
#include <boost/assert.hpp>
#include <adjust/Adjust/AdjustPlayStoreSubscription2dx.h>
#include <adjust/Adjust/Adjust2dx.h>
#include "Adjust/OPAdjust.hpp"
#include "../../Manager.h"
#include "../../ManagerDelegate.h"
#include "../../Product.h"
#include "../../ProductConsumable.h"
#include "../../ProductSubscription.hpp"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"

namespace opalib {
namespace iap {

/**
 * C++ -->> Java
 */

namespace backend {
namespace helper {

const char* const CLASS_NAME = "com/opalastudios/opalib/iap/Backend";
static opalib::iap::Manager* globalManager = NULL;

void callStaticVoidMethod(const char* name)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, name, "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

bool callStaticBoolMethod(const char* name)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, name, "()Z")) {
        bool result = (t.env->CallStaticBooleanMethod(t.classID, t.methodID) == JNI_TRUE);
        t.env->DeleteLocalRef(t.classID);
        return result;
    } else {
        return false;
    }
}

void callStaticVoidMethodWithString(const char* name, const char* argument)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, name, "(Ljava/lang/String;)V")) {
        jstring jProductId = t.env->NewStringUTF(argument);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jProductId);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jProductId);
    }
}

void callStaticVoidMethodWithStringAndBool(const char* name, const char* argument, const bool flag)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, name, "(Ljava/lang/String;Z)V")) {
        jstring jProductId = t.env->NewStringUTF(argument);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jProductId, flag);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jProductId);
    }
}

} // namespace helper
} // namespace backend

/**
 * Java -->> C++
 */

extern "C"
{
    JNIEXPORT void JNICALL Java_com_opalastudios_opalib_iap_Backend_delegateOnServiceStarted(JNIEnv* env, jclass clazz)
    {
        using backend::helper::globalManager;
        BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

        if (globalManager && globalManager->delegate) {
            globalManager->delegate->onServiceStarted(globalManager);
        }
    }

    JNIEXPORT void JNICALL
    Java_com_opalastudios_opalib_iap_Backend_delegateOnPurchaseSucceed(JNIEnv* env, jclass clazz,
            jstring jproductId, jstring jorderId, jstring jsignature, jstring jpurchaseToken, jlong jpurchaseTime, jboolean jshouldtrack)
    {
        using backend::helper::globalManager;
        BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

        if (!globalManager) {
            return;
        }

        std::string productId = cocos2d::JniHelper::jstring2string(jproductId);
        auto product = globalManager->getProduct(productId.c_str());

        if (!product) {
            return;
        }

        std::string orderId = cocos2d::JniHelper::jstring2string(jorderId);
        std::string signature = cocos2d::JniHelper::jstring2string(jsignature);
        std::string purchaseToken = cocos2d::JniHelper::jstring2string(jpurchaseToken);

        ProductSubscription* productSubscription = dynamic_cast<ProductSubscription* const>(product);

        if (productSubscription) {
            productSubscription->setIsActive(true);
            // subscriptions are always tracked
            AdjustPlayStoreSubscription2dx subscription = AdjustPlayStoreSubscription2dx(
                    productSubscription->priceAmountMicro,
                    productSubscription->currencyCode,
                    productId,
                    orderId,
                    signature,
                    purchaseToken
            );
            subscription.setPurchaseTime(jpurchaseTime);

            Adjust2dx::trackPlayStoreSubscription(subscription);
        } else if (jshouldtrack) {
            if (OPAdjust::shared()->getConfig() &&
                OPAdjust::shared()->getInAppPurchaseToken().empty()) {
                // Tem que adicionar a key in_app_purchase_token no arquivo adjust.json
                assert(false);
            }
            if (!OPAdjust::shared()->getInAppPurchaseToken().empty()) {
                AdjustEvent2dx adjustEvent = AdjustEvent2dx(OPAdjust::shared()->getInAppPurchaseToken());
                adjustEvent.setRevenue(product->price, product->currencyCode);
                adjustEvent.setTransactionId(orderId);
                Adjust2dx::trackEvent(adjustEvent);
            }
        }

        product->onHasBeenPurchased();
        if (globalManager->delegate) {
            globalManager->delegate->onPurchaseSucceed(globalManager, product);
        }
    }

    JNIEXPORT void JNICALL Java_com_opalastudios_opalib_iap_Backend_delegateOnPurchaseFail(JNIEnv* env, jclass clazz)
    {
        using backend::helper::globalManager;
        BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

        if (globalManager && globalManager->delegate) {
            globalManager->delegate->onPurchaseFail(globalManager);
        }
    }

    JNIEXPORT void JNICALL Java_com_opalastudios_opalib_iap_Backend_delegateOnTransactionStart(JNIEnv* env, jclass clazz)
    {
        using backend::helper::globalManager;
        BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

        if (globalManager && globalManager->delegate) {
            globalManager->delegate->onTransactionStart(globalManager);
        }
    }

    JNIEXPORT void JNICALL Java_com_opalastudios_opalib_iap_Backend_delegateOnTransactionEnd(JNIEnv* env, jclass clazz)
    {
        using backend::helper::globalManager;
        BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

        if (globalManager && globalManager->delegate) {
            globalManager->delegate->onTransactionEnd(globalManager);
        }
    }

    JNIEXPORT void JNICALL Java_com_opalastudios_opalib_iap_Backend_onInitialized(JNIEnv* env, jclass clazz)
    {
        using backend::helper::globalManager;
        BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

        if (globalManager) {
            for (auto& row : globalManager->getProducts()) {
                bool isSubscription = (dynamic_cast<ProductSubscription* const>(row.second) != NULL);
                if (isSubscription) {
                    backend::helper::callStaticVoidMethodWithString(
                        "addSubscriptionItemDataRequest", row.second->getProductId().c_str());
                } else {
                    bool isConsumable = (dynamic_cast<ProductConsumable* const>(row.second) != NULL);
                    backend::helper::callStaticVoidMethodWithStringAndBool(
                        "addItemDataRequest", row.second->getProductId().c_str(), isConsumable);
                }
            }
        }
        backend::helper::callStaticVoidMethod("startItemDataRequest");
    }

    JNIEXPORT void JNICALL Java_com_opalastudios_opalib_iap_Backend_onItemData(
        JNIEnv* env, jclass clazz, jstring jProductId, jstring jName, jstring jDesc, jstring jPriceStr, jfloat jprice, jstring jcurrencyCode, jstring jpriceAmountMicro)
    {
        using backend::helper::globalManager;
        BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

        if (!globalManager) {
            return;
        }

        std::string productId = cocos2d::JniHelper::jstring2string(jProductId);
        auto product = globalManager->getProduct(productId.c_str());

        if (!product) {
            return;
        }

        std::string localizedName = cocos2d::JniHelper::jstring2string(jName);
        std::string localizedDescription = cocos2d::JniHelper::jstring2string(jDesc);
        std::string localizedPrice = cocos2d::JniHelper::jstring2string(jPriceStr);
        std::string currencyCode = cocos2d::JniHelper::jstring2string(jcurrencyCode);
        std::string priceAmountMicro = cocos2d::JniHelper::jstring2string(jpriceAmountMicro);

        product->localizedName = localizedName;
        product->localizedDescription = localizedDescription;
        product->localizedPrice = localizedPrice;
        product->price = (float)jprice;
        product->currencyCode = currencyCode;
        product->priceAmountMicro = priceAmountMicro;
    }

    JNIEXPORT void JNICALL Java_com_opalastudios_opalib_iap_Backend_onSubscriptionVerified(
            JNIEnv* env, jclass clazz, jstring jProductId, jboolean jIsActive)
    {
        using backend::helper::globalManager;
        BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

        if (!globalManager) {
            return;
        }

        std::string productId = cocos2d::JniHelper::jstring2string(jProductId);
        auto product = globalManager->getProductSubscription(productId.c_str());

        if (!product) {
            return;
        }

        product->setIsActive(jIsActive);

        if (globalManager->delegate) {
            globalManager->delegate->onSubscriptionVerified(globalManager, product);
        }
    }

} // extern "C"

/**
 * Public API
 */

Backend::Backend(Manager& manager) : manager(manager)
{
    backend::helper::globalManager = &manager;
}

Backend::~Backend()
{
    shutdown();
    backend::helper::globalManager = NULL;
}

void Backend::shutdown()
{
    backend::helper::callStaticVoidMethod("shutdown");
}

void Backend::initialize()
{
    cocos2d::JniHelper::callStaticVoidMethod(backend::helper::CLASS_NAME, "initialize");
}

bool Backend::isInitialized() const
{
    return backend::helper::callStaticBoolMethod("isInitialized");
}

bool Backend::isPurchaseReady() const
{
    return backend::helper::callStaticBoolMethod("isPurchaseReady");
}

void Backend::purchase(Product* const product)
{
    bool isSubscription = (dynamic_cast<ProductSubscription* const>(product) != NULL);
    if (isSubscription) {
        backend::helper::callStaticVoidMethodWithString("purchaseSubscription", product->getProductId().c_str());
    } else {
        bool isConsumable = (dynamic_cast<ProductConsumable* const>(product) != NULL);
        backend::helper::callStaticVoidMethodWithStringAndBool(
            "purchase", product->getProductId().c_str(), isConsumable);
    }
}

void Backend::verifySubscriptions() const {
    // this is not needed
}

void Backend::restorePurchases() const {
    // this is not needed
}

} // namespace iap
} // namespace opalib
