package com.rover.gui;

import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.BevelBorder;
import javax.swing.border.TitledBorder;

public class ConnectionPanel extends JPanel implements IInit{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1318969322823238757L;
	
	private JLabel lbIP = new JLabel("IP:");
	private JTextField tfIP = new JTextField();
	private JLabel lbPort = new JLabel("Port:");
	private JTextField tfPort = new JTextField();
	private JButton btConnect = new JButton("Connect");
	private JButton btDisconnect = new JButton("Disconnect");
	 
	

	public ConnectionPanel() {
		initComponents();
		setComponentsPosition();
		addActionHandlers();
	}

	@Override
	public void initComponents() {
		// TODO Auto-generated method stub
		
		this.setLayout(new GridBagLayout());
		this.setBorder(new TitledBorder(BorderFactory.createBevelBorder(BevelBorder.RAISED, Color.white, new Color(248, 254, 255),
				  new Color(84, 86, 95), new Color(121, 124, 136)), "Connection"));
		
		this.add(lbIP, new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		
		this.add(tfIP, new GridBagConstraints(1, 0, 3, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		
		this.add(lbPort, new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		
		this.add(tfPort, new GridBagConstraints(1, 1, 3, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		
		this.add(btConnect, new GridBagConstraints(0, 2, 2, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		
		this.add(btDisconnect, new GridBagConstraints(2, 2, 2, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		
	}

	@Override
	public void addActionHandlers() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setComponentsPosition() {
		// TODO Auto-generated method stub
		
	} 

}
