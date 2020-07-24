package medicalApplication.services;

import static org.junit.Assert.*;

import java.util.List;

import org.junit.Before;
import org.junit.Test;

import medical.com.medicalApplication.model.Doctor;
import medical.com.medicalApplication.services.DoctorService;

public class DoctorServiceTest {

	private static DoctorService testDoctorService = DoctorService.getReference();
	
	// Test getReference always returns the same reference
	@Test
	public void testGetReference() {
		// Each call to DoctorService.getService() should point to the same reference. Verify this with ==.
		// .equals compares object contents while == compares the memory location. In this case we should
		// get the same memory location.
		DoctorService funcLevel = DoctorService.getReference();
		assertTrue(testDoctorService == funcLevel);
	}
	
	// Test DoctorService::getAllDoctors
	@Test
	public void testGetAllDoctors() {
		List<Doctor> doctors = testDoctorService.getAllDoctors();
		assertTrue(doctors instanceof List);
	}
	
	// Test DoctorSErvice::addDoctor
	@Test
	public void testAddDoctor() {
		// Add a doctor to the DoctorService singleton data structure
		testDoctorService.addDoctor("TestName", "TestID");
		
		List<Doctor> doctors = testDoctorService.getAllDoctors();
		
		for (Doctor doctor : doctors) {
			if( doctor.getName().equals("TestName") && doctor.getId().equals("TestID")) {
				assertTrue(1==1);
				return;
			}
		}
		fail("testAddDoctor() : Doctor was not properly added to DoctorService Singleton.");
	}

}
