package medicalApplication.models;

import static org.junit.Assert.*;

import java.awt.List;
<<<<<<< HEAD
=======
import java.util.ArrayList;
>>>>>>> 5a259d1... Add JUnit tests to final project

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
<<<<<<< HEAD
=======
	
	// Test adding a medication before a treatment
	@Test
	public void testAddMedicationBeforeTreatment() {
		PatientHistory h = new PatientHistory();
		Medication m = new Medication("medicine", "start", "end", "dose");
		
		// this should fail since no treatment has been done
		h.addMedication(m);
		if( h.getAllMedications().contains(m)) {
			fail("Failure: Medication was added before a treatment. this should not happen");
		} else {
			assertTrue(true);
		}
	}
	
	// Test adding 1 to many medications post treatment
	@Test
	public void testAddManyMedications() {
		PatientHistory h = new PatientHistory();
		
		Treatment treatment = new Treatment("date", "diagnose", "description");
		Medication m1 = new Medication("medicine1", "start", "end", "dose");
		Medication m2 = new Medication("medicine2", "start", "end", "dose");
		Medication m3 = new Medication("medicine3", "start", "end", "dose");
		
		h.addTreatment(treatment);
		h.addMedication(m1);
		h.addMedication(m2);
		h.addMedication(m3);
		
		ArrayList<Medication> expectedMeds = new ArrayList<Medication>();
		
		expectedMeds.add(m1);
		expectedMeds.add(m2);
		expectedMeds.add(m3);
		
		assertEquals(expectedMeds, h.getAllMedications());
	}
>>>>>>> 5a259d1... Add JUnit tests to final project
}
