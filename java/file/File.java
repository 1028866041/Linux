import java.io.*;
import java.util.*;

public class File{

	private static ArrayList<Integer> list= new ArrayList<Integer>();
	/*private static HashMap<Integer, String> map= new HashMap<Integer, String>();*/
	public static void main(String args[]){

		try(Scanner sc= new Scanner(System.in)){ 
	        while(sc.hasNextInt()){  
	     		list.add(sc.nextInt());  
			}
		}

		File.writeData();
		File.readData(); //file.dat
	}

	public static void writeData(){		

		try{
			FileWriter file= new FileWriter("file.dat" ,true);
			BufferedWriter out= new BufferedWriter(file);
			for(int i=0;i< list.size();i++)
				out.write(list.get(i)+"\r\n");
			out.close();
		}catch(FileNotFoundException e){
			e.printStackTrace();
		}catch(IOException e){
			e.printStackTrace();
		}
		/*try(PrintWriter out=  new PrintWriter("file.dat","UTF-8")){
			for(int i=0;i< list.size();i++)
				out.println(list.get(i));
		}catch(FileNotFoundException e){
            e.printStackTrace();
        }catch(UnsupportedEncodingException e){
            e.printStackTrace();    
        }*/
		/*try(BufferedWriter out= new BufferedWriter(new OutputStreamWriter(
							new FileOutputStream("file.dat",true)))){
			for(int i=0;i< list.size();i++)
				out.write(list.get(i)+"\r\n");
		}catch(IOException e){
			e.printStackTrace();
		}*/
	}

	public static void  readData(){
        
		try(FileInputStream in= new FileInputStream("file.dat")){
			byte[] bytes= new byte[100];
			int byteread= 0;
			System.out.println(in.available());
			while((byteread= in.read(bytes))!= -1){
				System.out.write(bytes, 0, byteread);
			}
		}catch(IOException e){
			e.printStackTrace();
		}
	} 
}

