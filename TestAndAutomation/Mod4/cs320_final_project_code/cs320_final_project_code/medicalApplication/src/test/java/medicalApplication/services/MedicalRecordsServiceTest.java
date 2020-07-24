package medicalApplication.services;

import static org.junit.Assert.*;

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
		int numPatientsToTest = 30;
		Patient[] testPatients = new Patient[numPatientsToTest];
		Allergey[] allergies = {new Allergey("AllergyA"), new Allergey("AllergyB"), new Allergey("none")};
		
		// Populate our array 
		for(int x = 0; x < numPatientsToTest; x++) {
			testPatients[x] = new Patient("Name"+Integer.toString(x), "ID" + Integer.toString(x));
		}
		
		// Add our ptients to the MedicalRecordService.
		for( Patient patient : testPatients ) {
			testService.addPatient(patient.getName(), patient.getId());
		}
		// Give some patinets AllergyA, some AllergyB and some none
		for( int t = 0; t < testPatients.length; t++) {
			MedicalRecord mr = testService.getMedicalRecord(testPatients[t].getId());
			int allergyCase = t % 3;
			
			switch (allergyCase) {
				case 0:
					mr.getHistory().addAllergy(allergies[0]);
					break;
				case 1:
					mr.getHistory().addAllergy(allergies[1]);
					break;
				case 2:
					mr.getHistory().addAllergy(allergies[2]);
					break;
			}
		}
		MedicalRescordService newService = MedicalRescordService.getReference();
		// Get the number of patients with each allergy (should be 10, 10, 10)
		List<Patient> allergyACount = newService.getPatientsWithAllergies(allergies[0].getName());
		List<Patient> allergyBCount = newService.getPatientsWithAllergies(allergies[1].getName());
		List<Patient> noAllergyCount = newService.getPatientsWithAllergies(allergies[2].getName());
		
		System.out.println("AllergyA " + allergyACount);
		System.out.println("AllergyB " + allergyBCount);
		System.out.println("No allergy " + noAllergyCount);
		assertTrue( (allergyACount == 10) && (allergyACount == allergyBCount) && (allergyACount == noAllergyCount) );
	}
	
	
}
