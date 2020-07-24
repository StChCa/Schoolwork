package medicalApplication;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import medical.com.medicalApplication.model.MedicalRecord;
import medical.com.medicalApplication.model.Patient;
import medical.com.medicalApplication.model.PatientHistory;

public class MedicalRecordTest {

	Patient testPatient;
	MedicalRecord testMedicalRecord;
	
	@Before
	public void setUp() {
		testPatient = new Patient("PatientName", "PatientID");
		testMedicalRecord = new MedicalRecord(testPatient);
	}
	
	// Test MedicalRecord::getPatient()
	@Test
	public void testGetPatient() {
		// We expect to receive the class level patient objecet back
		Patient actual = testMedicalRecord.getPatient();
		
		assertEquals(testPatient, actual);
	}
	
	// Test MedicalRecord::getHistory()
	@Test
	public void testGetHistory() {
		// Verify that the getHistory function is returning an instance of PatientHistory.
		PatientHistory actual = testMedicalRecord.getHistory();
		
		assertTrue(actual instanceof PatientHistory);
	}

}
