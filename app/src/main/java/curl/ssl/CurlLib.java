package curl.ssl;

/**
 * Created by zhonglz on 16/8/6.
 */
public class CurlLib {
    private final static String TAG = CurlLib.class.getSimpleName();

    private static CurlLib sIns;

    private CurlLib(){

    }

    public static CurlLib getIns(){
        if (sIns == null){
            sIns = new CurlLib();
        }
        return sIns;
    }




    public native int init(String cert);
    public native byte[] testUrl(String url);

    static {
        System.loadLibrary("curlwithssl");
    }
}
