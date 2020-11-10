package com.gavinandre.uvccameralibrary;

import android.os.Build.VERSION;

public final class BuildCheck {
    public BuildCheck() {
    }

    private static final boolean check(int value) {
        return VERSION.SDK_INT >= value;
    }

    public static boolean isCurrentDevelopment() {
        return VERSION.SDK_INT == 10000;
    }

    public static boolean isBase() {
        return check(1);
    }

    public static boolean isBase11() {
        return check(2);
    }

    public static boolean isCupcake() {
        return check(3);
    }

    public static boolean isAndroid1_5() {
        return check(3);
    }

    public static boolean isDonut() {
        return check(4);
    }

    public static boolean isAndroid1_6() {
        return check(4);
    }

    public static boolean isEclair() {
        return check(5);
    }

    public static boolean isAndroid2_0() {
        return check(5);
    }

    public static boolean isEclair01() {
        return check(6);
    }

    public static boolean isEclairMR1() {
        return check(7);
    }

    public static boolean isFroyo() {
        return check(8);
    }

    public static boolean isAndroid2_2() {
        return check(8);
    }

    public static boolean isGingerBread() {
        return check(9);
    }

    public static boolean isAndroid2_3() {
        return check(9);
    }

    public static boolean isGingerBreadMR1() {
        return check(10);
    }

    public static boolean isAndroid2_3_3() {
        return check(10);
    }

    public static boolean isHoneyComb() {
        return check(11);
    }

    public static boolean isAndroid3() {
        return check(11);
    }

    public static boolean isHoneyCombMR1() {
        return check(12);
    }

    public static boolean isAndroid3_1() {
        return check(12);
    }

    public static boolean isHoneyCombMR2() {
        return check(13);
    }

    public static boolean isAndroid3_2() {
        return check(13);
    }

    public static boolean isIcecreamSandwich() {
        return check(14);
    }

    public static boolean isAndroid4() {
        return check(14);
    }

    public static boolean isIcecreamSandwichMR1() {
        return check(15);
    }

    public static boolean isAndroid4_0_3() {
        return check(15);
    }

    public static boolean isJellyBean() {
        return check(16);
    }

    public static boolean isAndroid4_1() {
        return check(16);
    }

    public static boolean isJellyBeanMr1() {
        return check(17);
    }

    public static boolean isAndroid4_2() {
        return check(17);
    }

    public static boolean isJellyBeanMR2() {
        return check(18);
    }

    public static boolean isAndroid4_3() {
        return check(18);
    }

    public static boolean isKitKat() {
        return check(19);
    }

    public static boolean isAndroid4_4() {
        return check(19);
    }

    public static boolean isKitKatWatch() {
        return VERSION.SDK_INT >= 20;
    }

    public static boolean isL() {
        return VERSION.SDK_INT >= 21;
    }

    public static boolean isLollipop() {
        return VERSION.SDK_INT >= 21;
    }

    public static boolean isAndroid5() {
        return check(21);
    }

    public static boolean isLollipopMR1() {
        return VERSION.SDK_INT >= 22;
    }

    public static boolean isM() {
        return check(23);
    }

    public static boolean isMarshmallow() {
        return check(23);
    }

    public static boolean isAndroid6() {
        return check(23);
    }

    public static boolean isN() {
        return check(24);
    }

    public static boolean isNougat() {
        return check(24);
    }

    public static boolean isAndroid7() {
        return check(24);
    }
}

