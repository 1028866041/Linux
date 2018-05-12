import java.io.*;
import java.net.*;
import java.nio.charset.Charset;
import java.nio.file.*;
import java.util.concurrent.*;
import java.util.logging.*;

public class Http{

	private static final Logger logger = Logger.getLogger("Http");
	private final byte[] content;
	private final byte[] header;
	private final int port;
	private final String  encoding;
	
	public Http(String data, String encoding,String mimeType, int port)
		throws UnsupportedEncodingException{
		this(data.getBytes(encoding), encoding, mimeType, port);
	}

	public Http(byte[] data, String encoding,String mimeType, int port){
		this.content= data;
		this.port= port;
		this.encoding= encoding;
		String header= "HTTP/1.0 200 OK\r\n"
				+"Server: OneFile 2.0\r\n"
				+"Content-length:"+ this.content.length +"\r\n"
				+"Conntent-type:"+mimeType +";charset="+ encoding+"\r\n";
		
		this.header= header.getBytes(Charset.forName("US-ASCII"));
	}

	public Void start(){
		/*Thread Pool processing*/
		ExecutorService pool= Executors.newFixedThreadPool(100);
		try(ServerSocket server= new ServerSocket(this.port)){
			logger.info("Accept connection on port"+ server.getLocalPort());
			logger.info("Date to send:"+(new String(this.content, encoding)));

			while(true){
				try{
					Socket connect= server.accept();
					pool.submit(new HttpHandler(connect));
				}catch(IOException e){
					logger.log(Level.SEVERE, "Unexcepted error",e);		
					e.printStackTrace();
				}catch(RuntimeException e){
					logger.log(Level.SEVERE, "Server runtime error",e);
					e.printStackTrace();
				}
			}	
		}catch(IOException e){
			logger.log(Level.SEVERE, "Server start error",e);	
			e.printStackTrace();
		}
		return null;
	}
	
	public class HttpHandler implements Callable<Void>{
		private final Socket connect;
		HttpHandler(Socket connect){
			this.connect= connect;
		}
		
		@Override
		public Void call() throws IOException{
			try{
				OutputStream out= new BufferedOutputStream(connect.getOutputStream());
				InputStream in= new BufferedInputStream(connect.getInputStream());
				StringBuilder request= new StringBuilder();
				while(true){
					int c= in.read();
					if(c=='\r'|| c=='\n'|| c==-1)
						break;				
					request.append((char)c);				
				}		

				if(request.toString().indexOf("HTTP/")!= -1){
					out.write(header);			
				}
				out.write(content);
				out.flush();			
			}catch(IOException e){
				logger.log(Level.WARNING, "Error writing",e);
				e.printStackTrace();
			}finally{
				connect.close();
			}
			return null;
		}
	}
	
	public static void main(String[] args){			
		int port;
		try{
			port= Integer.parseInt(args[1]);
			if(port<1|| port>65535)
				port= 80;
		}catch(RuntimeException e){
			port= 80;
			logger.severe(e.getMessage());	
		}

		String encoding= "UTF-8";
		if(args.length> 2)
			encoding= args[2];	
		try{
			Path path= Paths.get("127.0.0.1"/*args[0]*/);
			byte[] data= Files.readAllBytes(path);
			String contentType= URLConnection.getFileNameMap().getContentTypeFor("127.0.0.1"/*args[0]*/);

			Http server= new Http(data, encoding, contentType, port);
			server.start();			
		}catch(ArrayIndexOutOfBoundsException e){
			logger.severe(e.getMessage());	
			e.printStackTrace();
		}catch(IOException e){
			logger.severe(e.getMessage());
			e.printStackTrace();
		}
	}	
}

