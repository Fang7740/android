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
	private byte[] image_data;               // �洢ͼƬ������
	private final String upload_path = "http://192.168.1.120:8080/ImageServer/upServer";      //��������ַ
	int i;
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		button = (Button) this.findViewById(R.id.button1);                                   //�������������
		button2 = (Button) this.findViewById(R.id.button2);
		imageView = (ImageView) this.findViewById(R.id.imageView1);
		
		button.setOnClickListener(new View.OnClickListener() 
		{
			@Override
			public void onClick(View v) 
			{

				Intent intent = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);//�����
				startActivityForResult(intent, 1001); //��ʼ��
			}
		}); 
		
		button2.setOnClickListener(new View.OnClickListener()                               //�ϴ���Ƭ
		{
			private Bundle share;

			@Override
			
			public void onClick(View v) 
			{
				i++;
				SharedPreferences sharedPreferences = getSharedPreferences("wujay", Context.MODE_PRIVATE); //˽������
				Editor editor = sharedPreferences.edit();//��ȡ�༭��
				editor.putString("name", "wujaycode");
				editor.putInt("age", i);
				editor.commit();//�ύ�޸�
				
				//String value = sp.getString("age","Null");
				//Log.d("DAT", "DAT : " +value);

				HttpPostUtils.sendFormByPost(upload_path, image_data, ""+i);              //��������ַ,��Ƭ���ݣ���������              
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
			bitmap.compress(Bitmap.CompressFormat.PNG, 50, outputStream);  //ѹ��ͼƬ PNG��ʽ
		
			image_data = outputStream.toByteArray();                       //ͼƬ����
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
