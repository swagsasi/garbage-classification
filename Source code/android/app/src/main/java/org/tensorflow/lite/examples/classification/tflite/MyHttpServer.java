package org.tensorflow.lite.examples.classification.tflite;

import fi.iki.elonen.NanoHTTPD;


public class MyHttpServer extends NanoHTTPD {
    private String results;

    public MyHttpServer(int port) {
        super(port);
    }

    public void setResults(String results) {
        this.results = results;
        //Log.d("setResults", String.valueOf(results));
    }

    @Override
    public Response serve(IHTTPSession session) {
        //Log.d("serve", String.valueOf(results));
        if (results != null && !results.isEmpty()) {
            return newFixedLengthResponse(results);
        } else {
            String responseText = "No results available";
            return newFixedLengthResponse(responseText);
        }
    }
}


