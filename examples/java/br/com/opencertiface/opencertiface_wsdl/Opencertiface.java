/**
 * Opencertiface.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis 1.4 Oct 06, 2010 (02:39:27 GMT) WSDL2Java emitter.
 */

package br.com.opencertiface.opencertiface_wsdl;

public interface Opencertiface extends javax.xml.rpc.Service {

/**
 * service
 */
    public java.lang.String getopencertifaceAddress();

    public br.com.opencertiface.opencertiface_wsdl.OpencertifacePortType getopencertiface() throws javax.xml.rpc.ServiceException;

    public br.com.opencertiface.opencertiface_wsdl.OpencertifacePortType getopencertiface(java.net.URL portAddress) throws javax.xml.rpc.ServiceException;
}
