package com.example.android_camera_upload;

import java.io.ByteArrayOutputStream;

import android.os.Bundle;
import android.util.Log;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.graphics.Bitmap;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

public class MainActivity extends Activity {
	private SharedPreferences sp ;
	private Button button;                   //
	private Button button2;
	private ImageView imageView;
	private byte[] image_data;               // 存储图片的数据
	private final String upload_path = "http://192.168.1.120:8080/ImageServer/upServer";      //服务器地址
	int i;
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		button = (Button) this.findViewById(R.id.button1);                                   //按键打开相机拍照
		button2 = (Button) this.findViewById(R.id.button2);
		imageView = (ImageView) this.findViewById(R.id.imageView1);
		
		button.setOnClickListener(new View.OnClickListener() 
		{
			@Override
			public void onClick(View v) 
			{

				Intent intent = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);//打开相机
				startActivityForResult(intent, 1001); //初始化
			}
		}); 
		
		button2.setOnClickListener(new View.OnClickListener()                               //上传照片
		{
			private Bundle share;

			@Override
			
			public void onClick(View v) 
			{
				i++;
				SharedPreferences sharedPreferences = getSharedPreferences("wujay", Context.MODE_PRIVATE); //私有数据
				Editor editor = sharedPreferences.edit();//获取编辑器
				editor.putString("name", "wujaycode");
				editor.putInt("age", i);
				editor.commit();//提交修改
				
				//String value = sp.getString("age","Null");
				//Log.d("DAT", "DAT : " +value);

				HttpPostUtils.sendFormByPost(upload_path, image_data, ""+i);              //服务器地址,照片数据，保存名字              
			}
		});
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) 
	{
		super.onActivityResult(requestCode, resultCode, data);
		if (requestCode == 1001 && resultCode == RESULT_OK) 
		{
			Bundle bundle = data.getExtras();
			Bitmap bitmap = (Bitmap) bundle.get("data");
			ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
			bitmap.compress(Bitmap.CompressFormat.PNG, 50, outputStream);  //压缩图片 PNG格式
		
			image_data = outputStream.toByteArray();                       //图片数据
			imageView.setImageBitmap(bitmap);
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) 
	{
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
