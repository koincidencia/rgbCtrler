package com.rgbctrler.rgbctrler;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.graphics.Color;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import java.io.Serializable;
import java.net.InetAddress;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by kazy on 2017. 01. 30..
 */

//TODO: create rgbCtrler object
// - IP, hostname
// - Send color command
// - Send SSID,PASSWD command
// - Send hostname command
// - Send mode command
// - Get RSSI
// - Each object has an activity, so if it is clicked in the list,
//   then the corresponding activity could be called

public class RGBCtrler implements Serializable {
    protected InetAddress ip;
    protected String hostname;
    protected String urlBase;
    protected String urlReq;
    protected String urlSet;
    protected String urlRGB;

    RGBCtrler(InetAddress ip, String hostname){
        this.ip = ip;
        this.hostname = hostname;

        this.urlBase = "http://" + ip.getHostAddress();
        this.urlReq = this.urlBase + "/request";
        this.urlSet = this.urlBase + "/setup";
        this.urlRGB = this.urlBase + "/rgb";
    }

    public String GetHostname(){return hostname;}

    public void SendColor(final int color, Context context){
        StringRequest postRequest = new StringRequest(Request.Method.POST, urlRGB,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {

                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        error.printStackTrace();
                    }
                }
        ) {
            @Override
            protected Map<String, String> getParams()
            {

                Map<String, String>  params = new HashMap<>();
                // the POST parameters:
                params.put("r", String.valueOf(Color.red(color)));
                params.put("g", String.valueOf(Color.green(color)));
                params.put("b", String.valueOf(Color.blue(color)));
                params.put("intensity", "1.0");
                return params;
            }
        };
        Volley.newRequestQueue(context).add(postRequest);
    }

    public void SendSTAInfo(final String ssid, final String passwd, Context context){
        StringRequest postRequest = new StringRequest(Request.Method.POST, urlSet,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {

                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        error.printStackTrace();
                    }
                }
        ) {
            @Override
            protected Map<String, String> getParams()
            {
                Map<String, String>  params = new HashMap<>();
                // the POST parameters:
                params.put("ssid", ssid);
                params.put("password", passwd);
                return params;
            }
        };
        Volley.newRequestQueue(context).add(postRequest);
    }

    public void SendHostname(final String hostname, Context context){
        StringRequest postRequest = new StringRequest(Request.Method.POST, urlSet,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {

                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        error.printStackTrace();
                    }
                }
        ) {
            @Override
            protected Map<String, String> getParams()
            {
                Map<String, String>  params = new HashMap<>();
                // the POST parameters:
                params.put("hostname", hostname);
                return params;
            }
        };
        Volley.newRequestQueue(context).add(postRequest);
    }

    public void SetAPMode(Context context){
        StringRequest postRequest = new StringRequest(Request.Method.POST, urlSet,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {

                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        error.printStackTrace();
                    }
                }
        ) {
            @Override
            protected Map<String, String> getParams()
            {
                Map<String, String>  params = new HashMap<>();
                // the POST parameters:
                params.put("mode", "ap");
                return params;
            }
        };
        Volley.newRequestQueue(context).add(postRequest);
    }

    public void SetSTAMode(Context context){
        StringRequest postRequest = new StringRequest(Request.Method.POST, urlSet,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {

                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        error.printStackTrace();
                    }
                }
        ) {
            @Override
            protected Map<String, String> getParams()
            {
                Map<String, String>  params = new HashMap<>();
                // the POST parameters:
                params.put("mode", "sta");
                return params;
            }
        };
        Volley.newRequestQueue(context).add(postRequest);
    }
}
