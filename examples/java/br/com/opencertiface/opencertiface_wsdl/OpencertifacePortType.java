/**
 * OpencertifacePortType.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis 1.4 Oct 06, 2010 (02:39:27 GMT) WSDL2Java emitter.
 */

package br.com.opencertiface.opencertiface_wsdl;

public interface OpencertifacePortType extends java.rmi.Remote {

    /**
     * Service definition of function ns__ping
     */
    public java.lang.String ping(java.lang.String stringHello) throws java.rmi.RemoteException;

    /**
     * Service definition of function ns__verifyImg
     */
    public java.lang.String verifyImg(byte[] template1, byte[] template2, java.lang.String id) throws java.rmi.RemoteException;
}
