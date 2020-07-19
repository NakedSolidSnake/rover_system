package com.rover.gui;

import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.BevelBorder;
import javax.swing.border.TitledBorder;



public class SendPanel extends JPanel implements IInit{
	/**
	 * 
	 */
	private static final long serialVersionUID = -4238046933215213789L;	
	private JButton btSend = new JButton("Send");
	private JTextField tfSend = new JTextField();
	private JCheckBox cbNewLine =  new JCheckBox("New Line");
	private JCheckBox cbCarriageReturn =  new JCheckBox("Carriage Return");
	
	public SendPanel() {
		initComponents();
		setComponentsPosition();
		addActionHandlers();
	}

	@Override
	public void initComponents() {	
		
		this.setLayout(new GridBagLayout());
		this.setBorder(new TitledBorder(BorderFactory.createBevelBorder(BevelBorder.RAISED, Color.white, new Color(248, 254, 255),
				  new Color(84, 86, 95), new Color(121, 124, 136)), "Send Data"));
		
		this.add(tfSend, new GridBagConstraints(0, 0, 2, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		this.add(btSend, new GridBagConstraints(0, 1, 2, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));			
		
		this.add(cbNewLine, new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		
		this.add(cbCarriageReturn, new GridBagConstraints(1, 2, 1, 1, 0.0, 0.0,
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
