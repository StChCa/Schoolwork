package medicalApplication.models;

import static org.junit.Assert.*;

import java.awt.List;

import org.junit.Before;
import org.junit.Test;

import medical.com.medicalApplication.model.Allergey;
import medical.com.medicalApplication.model.Medication;
import medical.com.medicalApplication.model.PatientHistory;
import medical.com.medicalApplication.model.Treatment;

public class PatientHistoryTest {

	PatientHistory unchangingPatientHistory;
	PatientHistory changingPatientHistory;
	
	@Before
	public void setUp() {
		unchangingPatientHistory = new PatientHistory();
		changingPatientHistory = new PatientHistory();
	}
	
	// Test PatientHistory::addTreatment
	@Test
	public void testAddTreatment() {
		Treatment testTreatment = new Treatment("TestDate", "TestDiagnose", "TestDescription");
		changingPatientHistory.addTreatment(testTreatment);
		
		// Get the List<Treatment> of all treatments associated with this patient history
		java.util.List<Treatment> treatments = changingPatientHistory.getAllTreatments();
		
		// make sure that our "testTreatment" is included in the List of treatments.
		for (Treatment treatment : treatments) {
			if (treatment.equals(testTreatment)) {
				assertEquals(treatment, testTreatment);
				return;
			}
		}
		fail("Treatment was not added to PatientHistory.treatments");
	}

	// Test PatientHistory::addAllergy
	@Test
	public void testAddAllergy() {
		Allergey testAllergey = new Allergey("TestName");
		changingPatientHistory.addAllergy(testAllergey);
		
		// Get the List<Allergey> of all treatments associated with this patient history
		java.util.List<Allergey> alergies = changingPatientHistory.getAlergies();
		
		// make sure that our "testTreatment" is included in the List of treatments.
		for (Allergey allergey : alergies) {
			if (allergey.equals(testAllergey)) {
				assertEquals(allergey, testAllergey);
				return;
			}
		}
		fail("Allergey was not added to PatientHistory.Allergy list");
	}
	
	// Test PatientHistory::addMedication
	@Test
	public void testAddMedication() {
		Medication testMedication = new Medication("TestName", "TestStartDate", "TestEndDate", "TestDose");
		changingPatientHistory.addMedication(testMedication);
		
		// Get the List<Medicatiopn> of all treatments associated with this patient history
		java.util.List<Medication> medications = changingPatientHistory.getAllMedications();
		
		// make sure that our "testTreatment" is included in the List of treatments.
		for (Medication medication : medications) {
			if (medication.equals(testMedication)) {
				assertEquals(medication, testMedication);
				return;
			}
		}
		fail("Treatment was not added to PatientHistory.medications");
	}
}
