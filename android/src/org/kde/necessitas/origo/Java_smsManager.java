package org.kde.necessitas.origo;

import android.telephony.SmsManager;

public class Java_smsManager {

    public Java_smsManager(){}

    boolean sendTextMessage(String phone, String sms) {
        SmsManager smsManager = SmsManager.getDefault();
        smsManager.sendTextMessage(phone, null, sms, null, null);
        return true;
    }
}
