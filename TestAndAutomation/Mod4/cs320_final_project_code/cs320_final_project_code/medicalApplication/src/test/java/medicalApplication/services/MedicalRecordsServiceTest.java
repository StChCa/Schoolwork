package medicalApplication.services;

import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;

import org.junit.Test;

import medical.com.medicalApplication.model.Allergey;
import medical.com.medicalApplication.model.MedicalRecord;
import medical.com.medicalApplication.model.Medication;
import medical.com.medicalApplication.model.Patient;
import medical.com.medicalApplication.model.PatientHistory;
import medical.com.medicalApplication.model.Treatment;
import medical.com.medicalApplication.services.DoctorService;
import medical.com.medicalApplication.services.MedicalRescordService;

public class MedicalRecordsServiceTest {
	
	MedicalRescordService testService = MedicalRescordService.getReference();

	// Test getReference always returns the same reference
	@Test
	public void testGetReference() {
		// Each call to MedRecServ.getService() should point to the same reference. Verify this with ==.
		// .equals compares object contents while == compares the memory location. In this case we should
		// get the same memory location.
		MedicalRescordService funcLevel = MedicalRescordService.getReference();
		assertTrue(testService == funcLevel);
	}
	
	// Test "MedicalRescordService::addPatient returns true when patient is added
	// This test also verifies that a ptaint is added to the singleton's List<Patient> when the func is called.
	@Test
	public void testAddPatient() {
		boolean patientAdded = testService.addPatient("TFName", "TFID");
		
		// Verify that the patient was in fact added to the patient list
		List<Patient> patients = testService.getAllPatients();
		for (Patient patient : patients) {
			if( (patient.getName().equals("TFName")) && (patient.getId().equals("TFID")) ) {
				// The patient was successfully added, so patientAdded boolean shoudl be true.
				assertTrue(patientAdded == true);
				return;
			}
		}
		fail("patient failed to be added during the MedicalRecordService.addPatient(patient) function.");
	}
	
	// Test MedicalRescordService::getMedicalRecord
	@Test
	public void testGetMedicalRecord() {
		// We need to add a medical record to the singleton MedicalRecordsService.
		// To add a medical record, we need a patient and a patientHistory
		Patient testPatient = new Patient("PatientName", "PatientID");		
		
		// Add the patient to the records service (This generates a Medical record for this patient)
		testService.addPatient(testPatient.getName(), testPatient.getId());
		
		// Retrieve the medical record for this patient.
		MedicalRecord mr = testService.getMedicalRecord(testPatient.getId());
		
		Allergey ag = new Allergey("TestAllergy");
		Medication med = new Medication("TestMedicine", "TestStartDate", "TestEndDate", "TestDose");
		Treatment treat = new Treatment("TestDate", "TestDiagnose", "TestDescription");
		mr.getHistory().addAllergy(ag);
		mr.getHistory().getAllMedications().add(med);
		mr.getHistory().getAllTreatments().add(treat);
		
		// verify that the patient and patientHistory match what we input
		boolean recordMatch = true;
		
		// Verify the components of the patient and patient history.
		recordMatch = mr.getPatient().equals(testPatient) ? true : false;
		recordMatch = mr.getHistory().getAlergies().contains(ag) ? true :false;
		recordMatch = mr.getHistory().getAllMedications().contains(med) ? true :false;
		recordMatch = mr.getHistory().getAllTreatments().contains(treat) ? true :false;
		
		assertTrue(recordMatch);
	}
	
	// Test MedicalRecordService::getPatient
	@Test
	public void testGetPatient() {
		// Create a new patient for this test
		Patient expectedPatient = new Patient("PatentNameTest", "PatientIDTest");
		// Add this patient to the singleton
		testService.addPatient(expectedPatient.getName(), expectedPatient.getId());
		
		Patient actualPatient = testService.getPatient("PatientIDTest");
		
		assertEquals(expectedPatient.getId(), actualPatient.getId());
	}
	
	// Test MedicalRecord::getAllPatients()
	@Test
	public void testGetAllPatients() {
		assertTrue(testService.getAllPatients() instanceof List);
	}
	
	// Test MedicalRecordService::getPatientsWithAllergies
	@Test
	public void testGetPatientsWithAllergies() {
		// Create patients and allergies
		Patient p1 = new Patient("p1", "p1");
		Patient p2 = new Patient("p2", "p2");
		Patient p3 = new Patient("p3", "p3");
		Patient p4 = new Patient("p4", "p4");
		
		Allergey a1 = new Allergey("a1");
		Allergey a2 = new Allergey("a2");
		
		// create a dictionary mapping patients to allergies.
		Hashtable<Patient, Allergey> patientDict = new Hashtable<Patient, Allergey>();
		
		// Assign allergies to patients
		patientDict.put(p1, a1);
		patientDict.put(p2, a1);
		patientDict.put(p3, a1);
		patientDict.put(p4, a2);
		
		for(Patient key : patientDict.keySet()) {
			// Add patient to this medical record
			testService.getReference().addPatient(key.getName(), key.getId());
			// Get this patients medical record
			MedicalRecord mrRef = testService.getReference().getMedicalRecord(key.getId());
			
			if (mrRef != null) {
				mrRef.getHistory().addAllergy(patientDict.get(key));
			} else {
				fail("unable to add allergy to patient history.");
			}
		}
		
		// Create arrays of known patients with each allergy.
		List<Patient> expectedA1 = new ArrayList<Patient>();
		List<Patient> expectedA2 = new ArrayList<Patient>();
		
		expectedA1.add(p1);
		expectedA1.add(p2);
		expectedA1.add(p3);
		
		expectedA2.add(p4);
		
		List<Patient> actualA1 = new ArrayList<Patient>();
		List<Patient> actualA2 = new ArrayList<Patient>();
		
		try {
			for (Patient p : testService.getPatientsWithAllergies("a1")) {
				actualA1.add(p);
			}
			
			for (Patient p : testService.getPatientsWithAllergies("a2")) {
				actualA2.add(p);
			}			
		} catch(Exception e) {
			fail("Problem with retreiving list of patients with a given allergy");
		}

		assertTrue( (expectedA1.equals(actualA1)) && (expectedA2.equals(actualA2)) );
	}
	
	// Test getPatientsWithAllergies when 0 patients have this allergey
	@Test
	public void testGetPatientsWithAllergiesWhenNoneHaveAllergies() {
		Allergey noPatientAllergey = new Allergey("NoOneHasThis");
		
		List actual = null;
		try {
			actual = testService.getReference().getPatientsWithAllergies(noPatientAllergey.getName());	
		} catch( Exception e) {
			fail("Exception thrown when trying to access an allergey that no patients have");
		}
		
		List<Allergey> expected = new ArrayList<>();
		
		assertEquals(actual, expected);
	}
	
	
}
