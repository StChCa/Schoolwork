package schoolApplication.model;

import static org.junit.Assert.*;

import java.util.ArrayList;

import org.junit.Before;
import org.junit.Test;

import schoolApplication.app.model.Student;
import schoolApplication.app.model.Class;

public class TestStudent {
	private Student student;
	private ArrayList<Class> classes;
	
	@Before
	public void before(){
		this.student = new Student("Mike", "1111");
		this.classes = new ArrayList<Class>();
		classes.add(new Class("Software Design", "CS 101"));
		classes.add(new Class("Software Testing", "CS 201"));
	}
	@Test
	public void testSetName() {
		assertTrue(student.getName().equals("Mike"));
	}
	
	@Test
	public void testSetId() {
		assertTrue(student.getId().equals("1111"));
	}
	
	@Test
	public void testSettingCS101Classes(){
		student.addClass(new Class("Software Design", "CS 101"));	
		assertTrue(student.getClasses().stream().anyMatch(clazz -> clazz.getName().equals("Software Design") && clazz.getSection().equals("CS 101")));
	}
	
	//public void test

}
