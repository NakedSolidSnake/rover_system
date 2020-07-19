package com.rover.gui;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.border.BevelBorder;
import javax.swing.border.TitledBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class MotorControlPanel extends JPanel implements IInit{

	/**
	 * 
	 */
	private static final long serialVersionUID = -1907139072415596552L;

	private JPanel jpCommands;
	private JPanel jpPower;

	private JLabel lbPower = new JLabel();	
	private JSlider slider = new JSlider();

	JButton btStop = new JButton(new ImageIcon("images/stop.png"));
	JButton btForward = new JButton(new ImageIcon("images/up.png"));
	JButton btBackwards = new JButton(new ImageIcon("images/down.png"));
	JButton btLeft = new JButton(new ImageIcon("images/left.png"));
	JButton btRight = new JButton(new ImageIcon("images/right.png"));

	JButton bts[] = new JButton[] {
			btStop, btForward, btBackwards, btLeft, btRight
	};



	public MotorControlPanel() {
		initComponents();
		setComponentsPosition();
		addActionHandlers();
	}

	@Override
	public void initComponents() {
		this.setLayout(new GridBagLayout());
		this.setBorder(new TitledBorder(BorderFactory.createBevelBorder(BevelBorder.RAISED, Color.white, new Color(248, 254, 255),
				new Color(84, 86, 95), new Color(121, 124, 136)), "Motor Control"));

		jpCommands = new JPanel(new GridBagLayout());
		jpPower = new JPanel(new GridBagLayout());

		for(int i = 0; i < bts.length; i++)
		{
			bts[i].setPreferredSize(new Dimension(50,50));
			bts[i].setMinimumSize(new Dimension(50,50));
			bts[i].setMaximumSize(new Dimension(50,50));
		}

		jpCommands.add(btForward, new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		jpCommands.add(btLeft, new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		jpCommands.add(btStop, new GridBagConstraints(1, 1, 1, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		jpCommands.add(btRight, new GridBagConstraints(2, 1, 1, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		jpCommands.add(btBackwards, new GridBagConstraints(1, 2, 1, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));


		slider.setOrientation(JSlider.VERTICAL);
		lbPower.setText(slider.getValue() + "");

		jpPower.add(lbPower, new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));

		jpPower.add(slider, new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));


		this.add(jpCommands, new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));
		this.add(jpPower, new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0,
				GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 5, 5), 0, 0));


	}

	@Override
	public void addActionHandlers() {
		// TODO Auto-generated method stub

		slider.addChangeListener(new ChangeListener() {

			@Override
			public void stateChanged(ChangeEvent arg0) {
				// TODO Auto-generated method stub
				lbPower.setText(slider.getValue() + "");
			}
		});

		btBackwards.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				System.out.println("Backwards");
			}
		});

		btForward.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				System.out.println("Forward");
			}
		});

		btLeft.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				System.out.println("Left");
			}
		});

		btRight.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				System.out.println("Right");
			}
		});
		
		btStop.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				System.out.println("Stop");
			}
		});


	}

	@Override
	public void setComponentsPosition() {
		// TODO Auto-generated method stub

	}

}
