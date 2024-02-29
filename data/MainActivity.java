
package %package_name%;

import android.app.Activity;                                                    
import android.content.pm.ActivityInfo;                                         
import android.app.NativeActivity;                                              
import android.os.Environment;                                                  
import android.content.Intent;                                                  
import android.content.ClipData;                                                
import android.database.Cursor;                                                 
import android.graphics.BitmapFactory;                                          
import android.net.Uri;                                                         
import android.os.Bundle;                                                       
import android.provider.MediaStore;                                             
import android.util.DisplayMetrics;                                             
import android.content.ActivityNotFoundException;                               
import java.util.ArrayList;       

public class MainActivity extends NativeActivity
{
/*  static { System.loadLibrary("main"); } */

  public static native void haveresult(int rcode);
  public int getDisplayDPI()
  {
    DisplayMetrics dm = new DisplayMetrics();
    getWindowManager().getDefaultDisplay().getMetrics(dm);
    float xDpi = dm.xdpi;
    float yDpi = dm.ydpi;
    if (xDpi>yDpi) return (int) xDpi; else return (int) yDpi;
  }
                                                         
  public int getStatusBarHeight() 
  {
    int result= 0; 
    int resourceId= getResources().getIdentifier             
                          ("status_bar_height", "dimen", "android");
    if (resourceId > 0)                                 
    {                                                          
        result= getResources().getDimensionPixelSize(resourceId); 
    }                 
    return result;
  }

                                    
  public int getNavigationBarHeight()                                           
  {                                                                             
    int result= 0;                                                              
    int resourceId= getResources().getIdentifier                                
                          ("navigation_bar_height", "dimen", "android");        
    if (resourceId > 0)                                                         
    {                                                                           
        result= getResources().getDimensionPixelSize(resourceId);               
    }                                                                           
    return result;                                                              
  }                                        

  public String getDownloadDirectory()
  {
    return Environment
           .getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)
           .getAbsolutePath(); 
  }
}

