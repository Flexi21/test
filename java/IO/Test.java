import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
//zxs 20150522 Start
import java.io.InputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
//end

//added by xen for display other driver info 20160421
import java.io.File;
import java.io.InputStreamReader;
import java.io.FileNotFoundException;
import java.io.BufferedReader;

public class Test {

    public static boolean main(String[] args) {
         System.out.println("begin：");
         String fileHeadset = "/sys/devices/virtual/switch/h2w/state";
         String fileHeadset = "./a";	
         byte[] bufferHeadSet = new byte[1];

         try {
                 InputStream inStream = new FileInputStream(fileHeadset);
                 inStream.read(bufferHeadSet);
                 if (bufferHeadSet != null) {				
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			baos.write(bufferHeadSet);
			String fullString = baos.toString(); 
                        int i = Integer.parseInt(fullString);

                        System.out.println("end："+fullString+fileHeadset+"--"+i );
                        if(i != 0)
                          return true;// System.out.println("CRC ok");
		 }

			
	 } catch (Exception e) {
		e.printStackTrace();
         }
                          return true;// System.out.println("CRC ok");			
     }
        
}
