package com.example.android_camera_upload;

import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.mime.MultipartEntity;
import org.apache.http.entity.mime.content.ByteArrayBody;
import org.apache.http.impl.client.DefaultHttpClient;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

public class HttpPostUtils 
{

	public HttpPostUtils() 
	{
		// TODO Auto-generated constructor stub
	}

	/**
	 * 
	 * @param path
	 * @param fileBody
	 * @param fileName
	 * @return 
	 */
	static String fileName1;
	public static void sendFormByPost(final String path, final Object image_data,final String fileName) //�ϴ���ַ ���� ����
	{
		new Thread(new Runnable()  //�������߳�
		{
			@Override
			public void run() 
			{
				// TODO Auto-generated method stub
				try {
					HttpClient httpClient = new DefaultHttpClient();  
					HttpPost httpPost = new HttpPost(path);
					
					MultipartEntity entity = new MultipartEntity();
					fileName1=fileName+".PNG";                                                    //������Ϊ i++  ���� .png
					entity.addPart("myfile", new ByteArrayBody((byte[]) image_data,fileName1));   //����Ƭ�����ַ��͵�web����
					
					 //Toast toast =Toast.makeText(getApplicationContext(),"�����°汾", Toast.LENGTH_LONG);
					 //toast.show();
					Log.d("save", "saved in : " +fileName1);
					httpPost.setEntity(entity);
					httpClient.execute(httpPost);
				} catch (Exception e) 
				{
					// TODO: handle exception
					e.printStackTrace();
				}
			}

			private Context getApplicationContext() {
				// TODO Auto-generated method stub
				return null;
			}
		}).start();
	}
}
