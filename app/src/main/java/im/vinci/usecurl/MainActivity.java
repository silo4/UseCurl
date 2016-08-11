package im.vinci.usecurl;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;

import curl.ssl.CurlLib;

public class MainActivity extends AppCompatActivity {

    private final static String TAG = MainActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        String pem = saveCertPemFile();

        CurlLib.getIns().init(pem);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        assert (fab != null);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
//                byte[] buffer = CurlLib.getIns().testUrl("https://avs-alexa-na.amazon.com");
                byte[] buffer = CurlLib.getIns().testUrl("https://baidu.com");
                if (buffer == null || buffer.length == 0){
                    Logger.e(TAG, "buffer is empty");
                }else {
                    try {
                        String content = new String(buffer, "UTF-8");
                        Logger.i(TAG, "[%s]", content);
                    } catch (UnsupportedEncodingException e) {
                        Logger.e(TAG, "buffer to string failed[%s]", e.getMessage());
                        e.printStackTrace();
                    }
                }

            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private String saveCertPemFile()
    {
        Context context=getApplicationContext();
        String assetFileName="cacert.pem";

        if(context==null || !FileExistInAssets(assetFileName,context))
        {
            Logger.i(TAG, "Context is null or asset file doesnt exist");
            return null;
        }
        //destination path is data/data/packagename
        String destPath=getApplicationContext().getApplicationInfo().dataDir;
        String CertFilePath =destPath + "/" +assetFileName;
        File file = new File(CertFilePath);
        if(file.exists())
        {
            //delete file
            file.delete();
        }
        //copy to internal storage
        if(CopyAssets(context,assetFileName,CertFilePath)==1) return CertFilePath;

        return CertFilePath=null;

    }

    private int CopyAssets(Context context,String assetFileName, String toPath)
    {
        AssetManager assetManager = context.getAssets();
        InputStream in = null;
        OutputStream out = null;
        try {
            in = assetManager.open(assetFileName);
            new File(toPath).createNewFile();
            out = new FileOutputStream(toPath);
            byte[] buffer = new byte[1024];
            int read;
            while ((read = in.read(buffer)) != -1)
            {
                out.write(buffer, 0, read);
            }
            in.close();
            in = null;
            out.flush();
            out.close();
            out = null;
            return 1;
        } catch(Exception e) {
            Logger.e(TAG, "CopyAssets"+e.getMessage());

        }
        return 0;

    }

    private boolean FileExistInAssets(String fileName,Context context)
    {
        try {
            return Arrays.asList(context.getResources().getAssets().list("")).contains(fileName);
        } catch (IOException e) {
            // TODO Auto-generated catch block

            Logger.e(TAG, "FileExistInAssets"+e.getMessage());

        }
        return false;
    }
}
