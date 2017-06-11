package com.rgbctrler.rgbctrler;

import android.content.Intent;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.os.StrictMode;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

public class DeviceSelectionActivity extends AppCompatActivity {
    public final static String EXTRA_BUNDLE = "com.rgbctrler.rgbctrler.BUNDLE";
    protected String TAG = "DeviceSelectionActivity";
    protected int PORT_BROADCAST = 2390;
    protected int PORT_RECEIVE = 12345;
    protected int SOCKET_TIMEOUT = 1000;
    protected DatagramSocket socket;
    protected Vector<InetAddress> deviceInetAddrs = new Vector<>();
    protected List<String> deviceHostnames = new ArrayList<String>();
    protected ArrayAdapter<String> arrayAdapter;

    private ListView lv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_selection);

        lv = (ListView) findViewById(R.id.listViewDevices);

        // This is the array adapter, it takes the context of the activity as a
        // first parameter, the type of list view as a second parameter and your
        // array as a third parameter.
        arrayAdapter = new ArrayAdapter<String>(
                this,
                android.R.layout.simple_list_item_1,
                deviceHostnames);
        lv.setAdapter(arrayAdapter);

        lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                RGBCtrler rgbCtrler = new RGBCtrler(deviceInetAddrs.elementAt(i), deviceHostnames.get(i));
                Intent intent = new Intent(DeviceSelectionActivity.this, ColorPickingActivity.class);
                Bundle bundle = new Bundle();
                bundle.putSerializable("rgbctrler", rgbCtrler);
                intent.putExtra(EXTRA_BUNDLE, bundle);
                startActivity(intent);
            }
        });
    }

    protected void searchDevices(View view) {
        Log.i(TAG, "SerachDevices");
        sendBroadcast(String.valueOf(PORT_RECEIVE));
        receiveAnswers();
    }

    protected void sendBroadcast(String messageStr) {
        // Hack Prevent crash (sending should be done using an async task)
        StrictMode.ThreadPolicy policy = new   StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

        try {
            //Open a random port to send the package
            DatagramSocket socket = new DatagramSocket();
            socket.setBroadcast(true);
            byte[] sendData = messageStr.getBytes();
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, getBroadcastAddress(), PORT_BROADCAST);
            socket.send(sendPacket);
            System.out.println(getClass().getName() + "Broadcast packet sent to: " + getBroadcastAddress().getHostAddress());
        } catch (IOException e) {
            Log.e(TAG, "IOException: " + e.getMessage());
        }
    }

    protected void receiveAnswers()
    {
        try {
            //Keep a socket open to listen to all the UDP trafic that is destined for this port
            socket = new DatagramSocket(PORT_RECEIVE, InetAddress.getByName("0.0.0.0"));
            socket.setBroadcast(true);
            socket.setSoTimeout(SOCKET_TIMEOUT);
            arrayAdapter.clear();

            while (true) {
                Log.i(TAG, "Ready to receive broadcast packets!");

                //Receive a packet
                byte[] recvBuf = new byte[15000];
                DatagramPacket packet = new DatagramPacket(recvBuf, recvBuf.length);
                socket.receive(packet);
                socket.setSoTimeout(SOCKET_TIMEOUT);

                //Packet received
                Log.i(TAG, "Packet received from: " + packet.getAddress().getHostAddress());
                String data = new String(packet.getData()).trim();
                Log.i(TAG, "Packet received; data: " + data);

                //Parse data
                String[] strArr = data.split("-");

                //Add device IP to storage if data is valid

                if (strArr[0].equals("NyihahaMuhaha")) {
                    try {
                        InetAddress addr = InetAddress.getByName(strArr[1]);    //get ipaddr
                        deviceInetAddrs.add(addr);
                        deviceHostnames.add(strArr[2]);
                        arrayAdapter.notifyDataSetChanged();
                    }catch(IOException ex) {
                        Log.i(TAG, "Oops " + ex.getMessage());
                    }
                }
            }
        } catch (IOException ex) {
            Log.i(TAG, "Oops, timeout and " + ex.getMessage());
            socket.close();
        }
    }

    InetAddress getBroadcastAddress() throws IOException {
        WifiManager wifi = (WifiManager) getApplicationContext().getApplicationContext().getSystemService(WIFI_SERVICE);
        DhcpInfo dhcp = wifi.getDhcpInfo();
        // handle null somehow

        int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
        byte[] quads = new byte[4];
        for (int k = 0; k < 4; k++)
            quads[k] = (byte) ((broadcast >> k * 8) & 0xFF);
        return InetAddress.getByAddress(quads);
    }
}
