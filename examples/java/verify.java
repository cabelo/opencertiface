import br.com.opencertiface.opencertiface_wsdl.*;
import java.rmi.RemoteException;
import java.net.URL;
import java.io.*;
import java.lang.*;
import java.io.*;
import org.w3c.dom.*;


public class verify {
   public static void main(String [] args) throws Exception {
      String url = args[0];
      String id  = args[3];
      OpencertifaceLocator service = new OpencertifaceLocator();
      service.setopencertifaceEndpointAddress(url);
      OpencertifacePortType port = service.getopencertiface();

      try
      {
          byte[] imageBytes1 = null;
          byte[] imageBytes2 = null;

          System.out.println( "Load image [" + args[1] + "]");
          FileInputStream img1 = new FileInputStream( args[1]);
          imageBytes1 = new byte[ img1.available()];
          img1.read( imageBytes1);
          img1.close();

          System.out.println( "Load image [" + args[2] + "]");
          FileInputStream img2 = new FileInputStream( args[2]);
          imageBytes2 = new byte[ img2.available()];
          img2.read( imageBytes2);
          img2.close();

          String result= port.verifyImg(imageBytes1,imageBytes2,id);
          System.out.println( "Resultado: " + result);

      }
      catch( org.apache.axis.AxisFault e)
      {
         System.out.println( "AxisFault: " + e.getMessage());
         Element[] details = e.getFaultDetails();
         Node nodeDetails = details[0].getFirstChild();
         System.out.println( "AxisFault: " + nodeDetails.getNodeValue());
         System.out.println( "AxisFault: " + e.getFaultReason());
      }
      catch ( java.lang.Exception e)
      {
         e.printStackTrace();
      }
   }
}

