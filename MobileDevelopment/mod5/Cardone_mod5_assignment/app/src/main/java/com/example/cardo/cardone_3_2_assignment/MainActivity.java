package com.example.cardo.cardone_3_2_assignment;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import org.w3c.dom.Text;

public class MainActivity extends AppCompatActivity {

    private EditText _editTextName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        _editTextName = (EditText) findViewById(R.id.nameText);

        // Add a listener on _editTextName EditText. to update button state.
        _editTextName.addTextChangedListener(new TextWatcher() {
            public void afterTextChanged(Editable s) {
                setButtonStatus();
            }
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
                setButtonStatus();
            }
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                setButtonStatus();
            }
        });
    }

    // Hello button callback. This function executes whenever the user presses the "SAY HELLO" button.
    public void sayHello(View view) {
        TextView tv = (TextView) findViewById(R.id.textGreeting);
        String etString = _editTextName.getText().toString();
        System.out.println("et text = " + etString);
        if ( editTextIsNullOrBlank() ) {
            tv.setText("You must enter a name");
        } else {
            tv.setText("Hello " + etString);
        }
    }

    // Enable/Disable "SayHello" button based on EditText name field.
    public void setButtonStatus() {
        Button button = (Button) findViewById(R.id.buttonSayHello);
        if ( editTextIsNullOrBlank() ) {
            button.setEnabled(false);
        } else {
            button.setEnabled(true);
        }

    }

    // Null/blank check the Name EditText view
    private boolean editTextIsNullOrBlank() {
        String etString = _editTextName.getText().toString();
        if ( etString.equals("") || etString == null ) {
            return true;
        } else {
            return false;
        }
    }
}
