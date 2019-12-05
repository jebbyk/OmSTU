package com.example.androidweatherapp.Utils;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Iterator;

public class JsonUtils {
    public static int padd = 0;
    public static String json2text(JSONObject jsonObj) throws JSONException {
        padd++;
        //Iterating Key Set
        String result = "";
        Iterator<String> keys = jsonObj.keys();
        while (keys.hasNext()) {

            String key = (String)keys.next();
            Object valObj = jsonObj.get(key);

            //If next entry is Object
            if (valObj instanceof JSONObject) {
                // call printJSON on nested object
                result += json2text((JSONObject) valObj);
            }
            //iff next entry is nested Array
            else if (valObj instanceof JSONArray) {
                for(int i = 0; i < padd; i++)
                {
                    result += "  ";
                }
                result += ("arr: " + key);
                result += jsonArr2text((JSONArray) valObj);

            } else {

                for(int i = 0; i < padd; i++)
                {
                    result += "  ";
                }
                result += (key);
                float value = 0;
                String strValue = "";
                if(key.contains("temp")){
                    value = Float.parseFloat( valObj.toString());
                    strValue = Float.toString(value - 273.15f);
                }else{
                    strValue = valObj.toString();
                }
                result += (": " + strValue + "\n");
            }
        }
        padd--;
        return result;
    }

    public static String jsonArr2text(JSONArray jarray) throws JSONException {
        //Get Object from Array
        padd++;
        String result = "";
        for (int i = 0; i < jarray.length(); i++) {
            result += json2text(jarray.getJSONObject(i));
        }
        padd--;
        return result;
    }
}
