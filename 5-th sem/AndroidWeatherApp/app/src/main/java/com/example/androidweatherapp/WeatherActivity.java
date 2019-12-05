package com.example.androidweatherapp;

import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.net.URL;
import java.util.Iterator;

import static com.example.androidweatherapp.Utils.JsonUtils.json2text;
import static com.example.androidweatherapp.Utils.WebUtils.generateURL;
import static com.example.androidweatherapp.Utils.WebUtils.getResponceFromURL;
import static java.lang.System.in;

public class WeatherActivity extends AppCompatActivity {
    private EditText searchField;
    private Button searchButton;
    private TextView resultText;

    SharedPreferences sp;

    class OWQueryTask extends AsyncTask<URL, Void, String>{



        @Override
        protected String doInBackground(URL... urls) {
            String response = null;
            sp = getPreferences(MODE_PRIVATE);
            try {
                response = getResponceFromURL(urls[0]);
                SharedPreferences.Editor ed = sp.edit();
                ed.putString("data", response.toString());
                ed.commit();
            } catch (IOException e) {
                //e.printStackTrace();
                response = sp.getString("data", "no saved data");
            }

            return response;
        }

        protected void onPostExecute(String response){

            String result = null;
            try {
                  JSONObject jsonResponse = new JSONObject(response);
                  result = json2text(jsonResponse);
            } catch (JSONException e) {
                e.printStackTrace();
            }
            resultText.setText(result);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_weather);

        searchButton = findViewById(R.id.b_search_vk);
        searchField = findViewById(R.id.et_search_field);
        resultText = findViewById(R.id.tv_result);

        View.OnClickListener onClickListener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                URL generatedURL = generateURL(searchField.getText().toString());
//                resultText.setText(generatedURL.toString());
                new OWQueryTask().execute(generatedURL);

            }
        };

        searchButton.setOnClickListener(onClickListener);

    }

}
