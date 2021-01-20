package com.suxw.ndkfunc.util;

/**
 * @ClassName: StringUtil
 * @Description:
 * @Author: able
 * @Mail: g_su_xw@163.com
 * @Date: 2021/1/19
 */
public class StringUtil {

    static {
        System.loadLibrary("StringUtil");
    }

    public StringUtil(){};

    private static final String TAG = StringUtil.class.getSimpleName();

    public native String getOutString(String strInput);
}
