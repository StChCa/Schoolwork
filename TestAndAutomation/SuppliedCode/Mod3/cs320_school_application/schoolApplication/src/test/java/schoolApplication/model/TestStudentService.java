package schoolApplication.model;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import schoolApplication.app.service.StudentService;
import schoolApplication.app.model.Class;
import schoolApplication.app.model.Student;

public class TestStudentService {
	private Student student;
	@Before
	public void setUp() throws Exception {
		StudentService.getReference().createStudent("Mike", "1234");
		this.student = StudentService.getReference().getStudent("1234");
		//Assign class in the setUp method because there is no consistent order in running the tests
		StudentService.getReference().assignClass("1234", new Class("CS 210", "1221"));
	}
	
	@Test
	public void testCreateStudent() {
		assertTrue(student.getName().equals("Mike"));
	}
	
	@Test
	public void testAssignClass() {
		assertTrue(student.getClasses().stream().anyMatch(clazz -> clazz.getName().equals("CS 210")));
	}

	@Test
	public void testAssignGradeToStudent() {
		StudentService.getReference().assignClassGradeToStudent("1234", "CS 210", "1221", "B");
		assertTrue(student.getClasses().stream().anyMatch( clazz -> clazz.getName().equals("CS 210") && clazz.getGrade().equals("B")));
	}
	
	@Test
	public void testUpdateGrade() {
		StudentService.getReference().updateGrade("1234", "CS 210", "1221", "A");
		assertTrue(student.getClasses().stream().anyMatch( clazz -> clazz.getName().equals("CS 210") && clazz.getGrade().equals("A")));
	}
}
