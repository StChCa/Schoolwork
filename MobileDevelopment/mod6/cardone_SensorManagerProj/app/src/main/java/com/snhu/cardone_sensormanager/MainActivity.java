package com.snhu.cardone_sensormanager;

import androidx.appcompat.app.AppCompatActivity;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SensorManager sm = (SensorManager) getSystemService(getApplicationContext().SENSOR_SERVICE);

        if ( sm.getSensorList(Sensor.TYPE_ACCELEROMETER).size() != 0) {
            Sensor s = sm.getSensorList(Sensor.TYPE_ACCELEROMETER).get(0);
            sm.registerListener( (SensorEventListener) this, s, SensorManager.SENSOR_DELAY_FASTEST);
        }
    }

    public void onSensorChanged(SensorEvent event) {
        System.out.println(" SENSOR EVENT CALLBACK");
        System.out.println( event.values.toString() );
        TextView tv = findViewById(R.id.accelText);

        if ( event != null ) {
            float x = event.values[0];
            float y = event.values[1];
            float z = event.values[2];
            String valuesStr = "X : " + x + "\nY : " + y + "\nZ : " + z;

            tv.setText(valuesStr);
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {
        System.out.println(" SENSOR ACCURACY CALLBACK");
        System.out.println( " Accuracy : " + i);
    }
}