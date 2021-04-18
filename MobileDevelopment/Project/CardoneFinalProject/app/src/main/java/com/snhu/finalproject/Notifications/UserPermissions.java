package com.snhu.finalproject.Notifications;

import android.util.Log;

public class UserPermissions {

    static boolean SMSPermission = false;
    static String phoneNumber = "5555555";

    public static void setPhoneNumber(String number) {
        phoneNumber = number;
    }

    public static void setSMSPermission(boolean permission) {
        Log.d("PERMISSION", "SMS permission set to " + permission);
        UserPermissions.SMSPermission = permission;
    }

    public static boolean getSMSPermission() {
        return SMSPermission;
    }

    public static String getPhoneNumber() {
        return phoneNumber;
    }
}
