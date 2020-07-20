package schoolApplication.model;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

public class TestCar {
	// I have created an empty car class
	private Car car;

	// The before annotation gets called before the test run
	@Before
	public void setUp() throws Exception {
		this.car = new Car();
	}

	@Test
	public void testSetName() {
		String expectedValue = "Toyota";
		car.setName(expectedValue);
		String actualValue = car.getName();
		assertTrue(expectedValue.equals(actualValue));
	}
	
	@Test
	public void testSpeed(){
		String expectedValue = "50 mph";
		car.setSpeed(expectedValue);
		String actualValue = car.getSpeed();
		assertTrue("Speed test passed", expectedValue.equals(actualValue));
	}
}
