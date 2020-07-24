package medicalApplication.models;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import medical.com.medicalApplication.model.Employee;

public class EmployeeTest {

	Employee testEmployee;
	
	@Before
	public void setUp() {
		testEmployee = new Employee("TestName", "TestID");
	}
	
	// Test Employee::getName()
	@Test
	public void testGetName() {
		String expectedValue = "TestName";
		String actualValue = testEmployee.getName();
		
		assertEquals(expectedValue, actualValue);
	}
	
	// Test Employee::getId()
	@Test
	public void testGetId() {
		String expectedValue = "TestID";
		String actualValue = testEmployee.getId();
		
		assertEquals(expectedValue, actualValue);
	}
	
	// Test Doctor::getPassword
	@Test
	public void testSetId() {
		Employee passwordTestEmployee = new Employee("name", "OriginalID");
		
		String expected = "Open";
		String actual = passwordTestEmployee.getPassword();
		
		assertEquals(expected, actual);
	}
}
