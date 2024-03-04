package %package_name% ;

import android.content.pm.ActivityInfo;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;                                                  
import android.app.Activity;

import android.view.View;
import android.view.View.OnTouchListener;

import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.Surface;

import android.content.Context;
import android.util.DisplayMetrics;                                             
import android.util.AttributeSet;


public class MainActivity
    extends Activity 
    implements SurfaceHolder.Callback,
               View.OnTouchListener
                            {
SurfaceView  view;

native boolean setup();
native void surface_new(Surface s);
native void surface_del();
native void mouse_event(int code, int button, int x, int y);
   /* public native void haveresult(int rcode); */

@Override protected void onCreate(Bundle savedInstanceState) 
{
  super.onCreate(savedInstanceState);
  System.loadLibrary("main");
  setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

  view = new SurfaceView(this);
  view.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN);
  view.getHolder().addCallback(this);
  view.setOnTouchListener(this);
  setContentView(view);
  setup();
}

@Override protected void onPause()
{
  super.onPause();
}

@Override protected void onResume() 
{
  super.onResume();
}

public void surfaceCreated(SurfaceHolder holder)
{
   // do nothing, we will handle it in Changed()
}

public void surfaceChanged(SurfaceHolder holder, int format,
                           int width, int height)
{
  surface_new(holder.getSurface());
}

public void surfaceDestroyed(SurfaceHolder holder)
{
      surface_del();
}


public String getDownloadDirectory()
{
  return Environment
           .getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)
           .getAbsolutePath(); 
}

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
  int resourceId= getResources().getIdentifier("status_bar_height",
                                               "dimen", "android");
  if (resourceId > 0)          
  {                                                          
    result= getResources().getDimensionPixelSize(resourceId); 
  }                 
  return result;
}

                                    
public int getNavigationBarHeight()                                           
{                                                                             
  int result= 0;                                                              
  int resourceId= getResources().getIdentifier("navigation_bar_height",
                                               "dimen", "android");        
  if (resourceId > 0)
  { 
    result= getResources().getDimensionPixelSize(resourceId);               
  }
  return result;                                                              
}                      

static final int GFXEV_BUTTON_PRESS= 1;
static final int GFXEV_BUTTON_RELEASE= 2;
static final int GFXEV_POINTER_MOVE= 6;

private int getEventButton(MotionEvent event)
{
  switch(event.getActionButton())
  {
    case MotionEvent.BUTTON_PRIMARY: return 1;
    case MotionEvent.BUTTON_SECONDARY: return 2;
    case MotionEvent.BUTTON_TERTIARY: return 3;
  }
  return 1;
}

      /** handle case event.getAction ==
    public boolean onGenericMotionEvent(MotionEvent event)
    {
              ACTION_SCROLL 
              ACTION_HOVER_MOVE
          etc..
https://developer.android.com/reference/android/view/MotionEvent#ACTION_SCROLL
      return true;
    }
      **/

@Override public boolean onTouch(View view, MotionEvent event)
{
  int x, y, code, button;

  switch(event.getActionMasked())
  {
  case MotionEvent.ACTION_DOWN:
    code= GFXEV_BUTTON_PRESS;
    button= 1;
    break;
  case MotionEvent.ACTION_UP:
    code= GFXEV_BUTTON_RELEASE;
    button= 1;
    break;
  case MotionEvent.ACTION_MOVE:
    code= GFXEV_POINTER_MOVE;
    button= 0;
    break;
  case MotionEvent.ACTION_BUTTON_PRESS:
    code= GFXEV_BUTTON_PRESS;
    button= getEventButton(event);
    break;
  case MotionEvent.ACTION_BUTTON_RELEASE:
    code= GFXEV_BUTTON_RELEASE;
    button= getEventButton(event);
    break;
  default:
    return true;
  }
  x= (int) event.getX();
  y= (int) event.getY();

  mouse_event(code, button, x, y);
  return true;
}
    
                            }              /** CLASS MainActivity **/

