package com.rgbctrler.rgbctrler;

import android.content.DialogInterface;
import android.content.Intent;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.InputType;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
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

    public boolean onCreateOptionsMenu(Menu menu) {

        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.rgbctrler_options, menu);
        return true;
    }

    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
            case R.id.opt_dev_name:
                setDeviceNameDialog();
                return true;
            case R.id.opt_wifi_sta:
                return true;
            case R.id.opt_wifi_ap:
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    protected void setDeviceNameDialog() {
        final EditText input = new EditText(this);
        input.setInputType(InputType.TYPE_CLASS_TEXT);

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Device name setup");
        builder.setMessage("Alert message to be shown");
        builder.setView(input);
        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                String devName =  input.getText().toString();
                rgbCtrler.SendHostname(devName, getApplicationContext());
                dialog.dismiss();
            }
        });
        builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
            }
        });
        builder.show();
    }
}
