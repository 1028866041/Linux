import java.io.*;
import java.util.*;
import java.util.zip.*;

public class Zip{
	
	public static void main(String args[]){

		try{
			FileOutputStream fout= new FileOutputStream("file.zip");
			ZipOutputStream zout= new ZipOutputStream(fout);
			byte data[] = new byte[1024];
			int count;
				
			zout.putNextEntry(new ZipEntry("file.dat"));
			BufferedInputStream buff= new BufferedInputStream(new FileInputStream("file.dat"));
			while((count= buff.read(data, 0, 1024))!= -1)         
                zout.write(data, 0, count);    
            
			zout.closeEntry();			
			buff.close();  
			zout.close();
			fout.close();
		}catch(FileNotFoundException e){
			e.printStackTrace();
		}catch(IOException e){
			e.printStackTrace();
		}		
	}
}

