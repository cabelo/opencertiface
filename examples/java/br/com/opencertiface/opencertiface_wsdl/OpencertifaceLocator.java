/**
 * OpencertifaceLocator.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis 1.4 Oct 06, 2010 (02:39:27 GMT) WSDL2Java emitter.
 */

package br.com.opencertiface.opencertiface_wsdl;

public class OpencertifaceLocator extends org.apache.axis.client.Service implements br.com.opencertiface.opencertiface_wsdl.Opencertiface {

/**
 * service
 */

    public OpencertifaceLocator() {
    }


    public OpencertifaceLocator(org.apache.axis.EngineConfiguration config) {
        super(config);
    }

    public OpencertifaceLocator(java.lang.String wsdlLoc, javax.xml.namespace.QName sName) throws javax.xml.rpc.ServiceException {
        super(wsdlLoc, sName);
    }

    // Use to get a proxy class for opencertiface
    private java.lang.String opencertiface_address = "http://opencertiface.com.br/opencertiface.cgi";

    public java.lang.String getopencertifaceAddress() {
        return opencertiface_address;
    }

    // The WSDD service name defaults to the port name.
    private java.lang.String opencertifaceWSDDServiceName = "opencertiface";

    public java.lang.String getopencertifaceWSDDServiceName() {
        return opencertifaceWSDDServiceName;
    }

    public void setopencertifaceWSDDServiceName(java.lang.String name) {
        opencertifaceWSDDServiceName = name;
    }

    public br.com.opencertiface.opencertiface_wsdl.OpencertifacePortType getopencertiface() throws javax.xml.rpc.ServiceException {
       java.net.URL endpoint;
        try {
            endpoint = new java.net.URL(opencertiface_address);
        }
        catch (java.net.MalformedURLException e) {
            throw new javax.xml.rpc.ServiceException(e);
        }
        return getopencertiface(endpoint);
    }

    public br.com.opencertiface.opencertiface_wsdl.OpencertifacePortType getopencertiface(java.net.URL portAddress) throws javax.xml.rpc.ServiceException {
        try {
            br.com.opencertiface.opencertiface_wsdl.OpencertifaceStub _stub = new br.com.opencertiface.opencertiface_wsdl.OpencertifaceStub(portAddress, this);
            _stub.setPortName(getopencertifaceWSDDServiceName());
            return _stub;
        }
        catch (org.apache.axis.AxisFault e) {
            return null;
        }
    }

    public void setopencertifaceEndpointAddress(java.lang.String address) {
        opencertiface_address = address;
    }

    /**
     * For the given interface, get the stub implementation.
     * If this service has no port for the given interface,
     * then ServiceException is thrown.
     */
    public java.rmi.Remote getPort(Class serviceEndpointInterface) throws javax.xml.rpc.ServiceException {
        try {
            if (br.com.opencertiface.opencertiface_wsdl.OpencertifacePortType.class.isAssignableFrom(serviceEndpointInterface)) {
                br.com.opencertiface.opencertiface_wsdl.OpencertifaceStub _stub = new br.com.opencertiface.opencertiface_wsdl.OpencertifaceStub(new java.net.URL(opencertiface_address), this);
                _stub.setPortName(getopencertifaceWSDDServiceName());
                return _stub;
            }
        }
        catch (java.lang.Throwable t) {
            throw new javax.xml.rpc.ServiceException(t);
        }
        throw new javax.xml.rpc.ServiceException("There is no stub implementation for the interface:  " + (serviceEndpointInterface == null ? "null" : serviceEndpointInterface.getName()));
    }

    /**
     * For the given interface, get the stub implementation.
     * If this service has no port for the given interface,
     * then ServiceException is thrown.
     */
    public java.rmi.Remote getPort(javax.xml.namespace.QName portName, Class serviceEndpointInterface) throws javax.xml.rpc.ServiceException {
        if (portName == null) {
            return getPort(serviceEndpointInterface);
        }
        java.lang.String inputPortName = portName.getLocalPart();
        if ("opencertiface".equals(inputPortName)) {
            return getopencertiface();
        }
        else  {
            java.rmi.Remote _stub = getPort(serviceEndpointInterface);
            ((org.apache.axis.client.Stub) _stub).setPortName(portName);
            return _stub;
        }
    }

    public javax.xml.namespace.QName getServiceName() {
        return new javax.xml.namespace.QName("http://opencertiface.com.br/opencertiface.wsdl", "opencertiface");
    }

    private java.util.HashSet ports = null;

    public java.util.Iterator getPorts() {
        if (ports == null) {
            ports = new java.util.HashSet();
            ports.add(new javax.xml.namespace.QName("http://opencertiface.com.br/opencertiface.wsdl", "opencertiface"));
        }
        return ports.iterator();
    }

    /**
    * Set the endpoint address for the specified port name.
    */
    public void setEndpointAddress(java.lang.String portName, java.lang.String address) throws javax.xml.rpc.ServiceException {
        
if ("opencertiface".equals(portName)) {
            setopencertifaceEndpointAddress(address);
        }
        else 
{ // Unknown Port Name
            throw new javax.xml.rpc.ServiceException(" Cannot set Endpoint Address for Unknown Port" + portName);
        }
    }

    /**
    * Set the endpoint address for the specified port name.
    */
    public void setEndpointAddress(javax.xml.namespace.QName portName, java.lang.String address) throws javax.xml.rpc.ServiceException {
        setEndpointAddress(portName.getLocalPart(), address);
    }

}
