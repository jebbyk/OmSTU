package com.example.androidweatherapp.Utils;

import android.net.Uri;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Scanner;

public class WebUtils {

    private static final String OW_API_BASE_URL = "http://api.openweathermap.org/data/2.5/";
    private static final String OW_API_METHOD = "weather";
    private static final String OW_PARAM_CITY = "q";

    private static final String OW_PARAM_RESPONSE_TYPE = "mode";
    private static final String OW_RESPONSE_TYPE = "xml";

    private static final String OW_PARAM_KEY = "APPID";
    private static final String OW_KEY = "368e827be4b38db51ff960ca88b5c396";


    public static URL generateURL(String cityName){
        Uri builtUri = Uri.parse(OW_API_BASE_URL + OW_API_METHOD).buildUpon()
                .appendQueryParameter(OW_PARAM_CITY, cityName)
                //.appendQueryParameter(OW_PARAM_RESPONSE_TYPE, OW_RESPONSE_TYPE)
                .appendQueryParameter(OW_PARAM_KEY, OW_KEY)
                .build();

        URL url = null;
        try {
            url = new URL(builtUri.toString());
        } catch (MalformedURLException e) {
            e.printStackTrace();
        }

        return url;
    }


    public static String getResponceFromURL(URL url) throws IOException {
        HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();

        try {
            InputStream in = urlConnection.getInputStream();
            Scanner scanner = new Scanner(in);
            scanner.useDelimiter("\\A");

            boolean hasInput = scanner.hasNext();

            if (hasInput) {
                return scanner.next();
            }else{
                return null;
            }
        }finally {
            urlConnection.disconnect();
        }
    }
}
