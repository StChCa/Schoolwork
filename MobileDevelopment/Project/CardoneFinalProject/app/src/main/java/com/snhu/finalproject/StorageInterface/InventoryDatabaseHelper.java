package com.snhu.finalproject.StorageInterface;

import android.Manifest;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.telephony.SmsManager;
import android.util.Log;
import android.widget.Toast;

import com.snhu.finalproject.Notifications.UserPermissions;

import androidx.annotation.Nullable;

public class InventoryDatabaseHelper extends SQLiteOpenHelper {

    private static final String TAG = "InventoryDatabaseHelper";

    private static final String TABLE_NAME = "inventory_table";
    private static final String COL1 = "part_number";
    private static final String COL2 = "quantity";

    public InventoryDatabaseHelper(@Nullable Context context) {
        super(context, TABLE_NAME, null, 1);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        String createTable = "CREATE TABLE " + TABLE_NAME + " (" + COL1 + " INTEGER PRIMARY KEY, " + COL2 + " INTEGER)";
        db.execSQL(createTable);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int i, int i1) {
        db.execSQL("DROP IF TABLE EXISTS " + TABLE_NAME);
        onCreate(db);
    }

    public boolean addData(int partNumber, int quantity) {
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues contentValues = new ContentValues();
        contentValues.put(COL1, partNumber);
        contentValues.put(COL2, quantity);

        Log.d(TAG, "addData: Adding item: " + partNumber + " pw: " + quantity +" to " + TABLE_NAME );

        long result = db.insert( TABLE_NAME, null, contentValues );
        Log.d(TAG, "insert result = " + result);

        if (result == -1 ) {
            return false;
        } else {
            return true;
        }
    }

    public boolean removeData(int partNumber) {
        if ( checkExists(partNumber)) {
            SQLiteDatabase db = this.getWritableDatabase();
            return db.delete(TABLE_NAME, COL1 + "=" + partNumber, null) > 0;
        } else {
            return false;
        }
    }

    public boolean incrementData( int partNumber, int change ) {
        if ( checkExists(partNumber) ) {
            SQLiteDatabase db = this.getWritableDatabase();
            String query = "UPDATE " + TABLE_NAME + " SET " + COL2 + " = " + COL2 + " + " + change +" WHERE " + COL1 + " = " + partNumber;
            db.execSQL(query);
            return true;
        } else {
            return false;
        }
    }

    public boolean decrementData( int partNumber, int change ) {
        if ( checkExists(partNumber) ) {
            SQLiteDatabase db = this.getWritableDatabase();
            String query = "UPDATE " + TABLE_NAME + " SET " + COL2 + " = " + COL2 + " - " + change +" WHERE " + COL1 + " = " + partNumber;
            db.execSQL(query);

            // check for 0 or less and notify user.
            if ( inventoryDropToZero(partNumber) ) {
                if ( UserPermissions.getSMSPermission() ) {
                    notifyUser(partNumber);
                }
            }
            return true;
        } else {
            return false;
        }
    }

    private void notifyUser(int partNumber) {
        Log.d(TAG, "NOTIFY UESR OF 0 INVENTORY");
        SmsManager mySmsManager = SmsManager.getDefault();
        String number = UserPermissions.getPhoneNumber();
        String message = "Part: " + partNumber + " has dropped below 0 stock";
        mySmsManager.sendTextMessage(number, null, message, null, null);
    }

    private boolean inventoryDropToZero(int partNumber) {
        SQLiteDatabase db = this.getReadableDatabase();
        String query = "SELECT * FROM " + TABLE_NAME + " WHERE " + COL1 + " = " + partNumber;
        Cursor c = db.rawQuery(query, null);

        if ( c.moveToFirst() ) {
            String quantityS = c.getString(1 );

            if ( Integer.parseInt(quantityS) <= 0 ) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }

    }

    public boolean checkExists( int partNumber) {
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor c = db.rawQuery("SELECT * FROM inventory_table WHERE part_number = ?", new String[] {String.valueOf(partNumber)});

        if ( c.getCount() > 0 ) {
            return true;
        } else {
            return false;
        }
    }

    public Cursor getData() {
        SQLiteDatabase db = this.getReadableDatabase();
        String query = "SELECT * FROM " + TABLE_NAME;
        Cursor data = db.rawQuery(query, null);
        return data;
    }
}
