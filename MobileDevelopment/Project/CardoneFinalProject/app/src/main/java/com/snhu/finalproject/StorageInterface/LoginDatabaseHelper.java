package com.snhu.finalproject.StorageInterface;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

import androidx.annotation.Nullable;

public class LoginDatabaseHelper extends SQLiteOpenHelper {

    private static final String TAG = "LoginDatabaseHelper";

    private static final String TABLE_NAME = "accounts_table";
    private static final String COL1 = "ID";
    private static final String COL2 = "username";
    private static final String COL3 = "password";

    public LoginDatabaseHelper(@Nullable Context context) {
        super(context, TABLE_NAME, null, 1);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        String createTable = "CREATE TABLE " + TABLE_NAME + " (ID INTEGER PRIMARY KEY AUTOINCREMENT, " + COL2 + " TEXT, " + COL3 + " TEXT)";
        db.execSQL(createTable);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int i, int i1) {
        db.execSQL("DROP IF TABLE EXISTS " + TABLE_NAME);
        onCreate(db);
    }

    public boolean addData(String username, String password) {
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues contentValues = new ContentValues();
        contentValues.put(COL2, username);
        contentValues.put(COL3, password);

        Log.d(TAG, "addData: Adding user: " + username + " pw: " + password + " to " + TABLE_NAME );

        long result = db.insert( TABLE_NAME, null, contentValues );
        Log.d(TAG, "insert result = " + result);

        if (result == -1 ) {
            return false;
        } else {
            return true;
        }
    }

    public boolean checkExists( String username, String password) {
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor c = db.rawQuery("SELECT * FROM accounts_table WHERE username = ? AND password = ?", new String[] {username, password});

        if ( c.getCount() > 0 ) {
            return true;
        } else {
            return false;
        }
    }
}
