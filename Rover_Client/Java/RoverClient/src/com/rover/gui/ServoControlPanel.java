package com.rover.gui;

import java.awt.Color;
import java.awt.GridBagLayout;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.border.BevelBorder;
import javax.swing.border.TitledBorder;

public class ServoControlPanel extends JPanel implements IInit{

	/**
	 * 
	 */
	private static final long serialVersionUID = 7651520817258019370L;
	
	public ServoControlPanel() {
		initComponents();
		setComponentsPosition();
		addActionHandlers();
	}

	@Override
	public void initComponents() {
		this.setLayout(new GridBagLayout());
		this.setBorder(new TitledBorder(BorderFactory.createBevelBorder(BevelBorder.RAISED, Color.white, new Color(248, 254, 255),
				  new Color(84, 86, 95), new Color(121, 124, 136)), "Servo Control"));
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
