package com.suxw.ndkfunc.util;

/**
 * @ClassName: SerialPort
 * @Description:
 * @Author: able
 * @Mail: g_su_xw@163.com
 * @Date: 2021/1/26
 */
public class SerialPort {

    static {
        System.loadLibrary("SerialPort");
    }

    public native String getStringFromCpp(String strInput);

    public native void testSerialPort();

}
