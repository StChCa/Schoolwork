package com.snhu.finalproject;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.Toast;

import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.snhu.finalproject.Notifications.UserPermissions;
import com.snhu.finalproject.StorageInterface.InventoryDatabaseHelper;
import com.snhu.finalproject.StorageInterface.LoginDatabaseHelper;

import java.util.ArrayList;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

public class MainActivity extends AppCompatActivity {

    private boolean _loggedIn = false;
    private LoginDatabaseHelper loginDB;
    private InventoryDatabaseHelper inventoryDB;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        loginDB = new LoginDatabaseHelper(this);
        inventoryDB = new InventoryDatabaseHelper( this );

        ActivityCompat.requestPermissions(MainActivity.this, new String[] {Manifest.permission.SEND_SMS, Manifest.permission.READ_SMS}, PackageManager.PERMISSION_GRANTED);

        if ( _loggedIn) {
            switchToPrimaryFlow();
        } else {
            setContentView(R.layout.login);
        }
    }

    protected void switchToPrimaryFlow() {
        int permissionState = this.checkSelfPermission(Manifest.permission.SEND_SMS);
        Log.d("PERMISSION", "Actual permission enum: " + permissionState);
        if (permissionState == PackageManager.PERMISSION_GRANTED) {
            UserPermissions.setSMSPermission(true);
        }
        setContentView(R.layout.activity_main);
        BottomNavigationView navView = findViewById(R.id.nav_view);
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        AppBarConfiguration appBarConfiguration = new AppBarConfiguration.Builder(
                R.id.navigation_inventory, R.id.navigation_add_remove, R.id.navigation_increase_decrease)
                .build();
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment);
        NavigationUI.setupActionBarWithNavController(this, navController, appBarConfiguration);
        NavigationUI.setupWithNavController(navView, navController);
    }

    public void createAccount(View view) {
        EditText usernameET = findViewById(R.id.editTextTextPersonName);
        EditText passwordET = findViewById(R.id.editTextTextPassword);
        EditText phoneET = findViewById(R.id.editTextPhone);

        String username = usernameET.getText().toString();
        String password = passwordET.getText().toString();
        String phoneNumber = phoneET.getText().toString();

        boolean success = loginDB.addData(username, password);

        if ( success ) {
            UserPermissions.setPhoneNumber(phoneNumber);
            toast("added success");
        } else {
            toast("failed to create account");
        }
    }

    public void login(View view) {

        EditText usernameET = findViewById(R.id.editTextTextPersonName);
        EditText passwordET = findViewById(R.id.editTextTextPassword);
        EditText phoneET = findViewById(R.id.editTextPhone);

        String username = usernameET.getText().toString();
        String password = passwordET.getText().toString();
        String phoneNumber = phoneET.getText().toString();
        // attempt to login. if success switchToPrimaryFlow
        boolean successfulLogin = loginDB.checkExists( username, password );

        if ( successfulLogin ) {
            UserPermissions.setPhoneNumber(phoneNumber);
            switchToPrimaryFlow();
        } else {
            toast("invalid username or password");
        }
        // else warn invalid login
    }

    // #########################  INVENTORY FUNCTIONS ############################################
    public void addToInventory(View view) {
        EditText partNumberET = findViewById(R.id.editTextPartNumber);
        EditText quantityET = findViewById(R.id.editTextQuantity);

        int partNumber  = Integer.parseInt(partNumberET.getText().toString());
        int quantity  = Integer.parseInt( quantityET.getText().toString());


        boolean success = inventoryDB.addData(partNumber, quantity);

        if ( success ) {
            toast( "Item added successfully");
            partNumberET.setText("");
            quantityET.setText("");
        } else {
            toast("failed to add item");
        }

    }

    public void removeFromInventory(View view) {
        EditText partNumberET = findViewById(R.id.editTextPartNumber);
        int partNumber = Integer.parseInt(partNumberET.getText().toString());
        boolean success = inventoryDB.removeData(partNumber);
        if ( success ) {
            toast ("item deleted successfully");
            partNumberET.setText("");
        } else {
            toast ("failed to delete");
        }
    }

    public void incrementInventoryItem( View view ) {
        EditText partNumberET = findViewById(R.id.editTextPartNumber);
        EditText quantityET = findViewById(R.id.editTextQuantity);
        int partNumber = Integer.parseInt(partNumberET.getText().toString());
        int quantity = Integer.parseInt(quantityET.getText().toString());
        boolean success = inventoryDB.incrementData(partNumber, quantity);
        if (success) {
            toast("" + partNumber + " quantity updated");
        } else {
            toast( "inventory update failed");
        }
    }

    public void decrementInventoryItem( View view ) {
        EditText partNumberET = findViewById(R.id.editTextPartNumber);
        EditText quantityET = findViewById(R.id.editTextQuantity);
        int partNumber = Integer.parseInt(partNumberET.getText().toString());
        int quantity = Integer.parseInt(quantityET.getText().toString());
        boolean success = inventoryDB.decrementData(partNumber, quantity);
        if (success) {
            toast("" + partNumber + " quantity updated");
        } else {
            toast( "inventory update failed");
        }
    }

    public void populateGridView(View view) {
        Log.d("MainActivity", "Display data in the grid");

        // Get the data and add it to the grid
        Cursor data = inventoryDB.getData();
        ArrayList<String> listData = new ArrayList<>();
        while(data.moveToNext()){
            listData.add(data.getString(0));
            listData.add(data.getString(1));
        }

        GridView gridView = (GridView) findViewById(R.id.gridInventory);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1, listData);
        gridView.setAdapter(adapter);
    }

    private void toast(String s) {
        Toast toast=Toast. makeText(getApplicationContext(), s ,Toast. LENGTH_SHORT);
        toast. setMargin(50,50);
        toast. show();
    }

}