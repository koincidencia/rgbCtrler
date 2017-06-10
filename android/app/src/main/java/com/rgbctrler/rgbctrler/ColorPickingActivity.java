package com.rgbctrler.rgbctrler;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import com.chiralcode.colorpicker.ColorPicker;

import java.util.List;

public class ColorPickingActivity extends AppCompatActivity {
    protected RGBCtrler rgbCtrler;
    protected ColorPicker colorPicker;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_color_picking);

        Intent intent = getIntent();
        Bundle bundle = intent.getBundleExtra(DeviceSelectionActivity.EXTRA_BUNDLE);
        rgbCtrler = (RGBCtrler) bundle.getSerializable("rgbctrler");

        colorPicker = (ColorPicker) findViewById(R.id.colorPicker);
        colorPicker.setOnTouchEventIface(new ColorPicker.OnTouchEventIface(){
            @Override
            public void customOnTouchEvent(){
                int color = colorPicker.getColor();
                rgbCtrler.SendColor(color, colorPicker.getContext());
            }
        });

    }

    public void setup(View view) {
        Intent intent = new Intent(ColorPickingActivity.this, SettingsActivity.class);
        startActivity(intent);
    }
}
