//
// Created by Ricardo on 11/25/2020.
//

#ifndef OP_VIBRATOR_HPP
#define OP_VIBRATOR_HPP

namespace opalib {

    class Vibrator {
    public:

        //implemented for each platform
        static Vibrator* createForPlatform();

        Vibrator();
        virtual ~Vibrator();
        void vibrate(int milliseconds);
        void setEnabled(bool enabled);

        inline bool isEnabled() { return _enabled; }
    protected:
        virtual void doVibration(int milliseconds) = 0;
    protected:
        bool _enabled = true;
    };
}

#endif //OP_VIBRATOR_HPP
