/***************************************************************************
                          addconstraintactivitiespreferredstartingtimesform.cpp  -  description
                             -------------------
    begin                : 15 May 2004
    copyright            : (C) 2004 by Lalescu Liviu
    email                : Please see https://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include <Qt>

#include <QMessageBox>

#include "longtextmessagebox.h"

#include "addconstraintactivitiespreferredstartingtimesform.h"
#include "timeconstraint.h"

#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QBrush>
#include <QColor>

#define YES		(QString(" "))
#define NO		(QString("X"))

AddConstraintActivitiesPreferredStartingTimesForm::AddConstraintActivitiesPreferredStartingTimesForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	durationCheckBox->setChecked(false);
	durationSpinBox->setEnabled(false);
	durationSpinBox->setMinimum(1);
	durationSpinBox->setMaximum(gt.rules.nHoursPerDay);
	durationSpinBox->setValue(1);

	addConstraintPushButton->setDefault(true);

	connect(preferredTimesTable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(itemClicked(QTableWidgetItem*)));
	connect(addConstraintPushButton, SIGNAL(clicked()), this, SLOT(addConstraint()));
	connect(closePushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(setAllAllowedPushButton, SIGNAL(clicked()), this, SLOT(setAllSlotsAllowed()));
	connect(setAllNotAllowedPushButton, SIGNAL(clicked()), this, SLOT(setAllSlotsNotAllowed()));

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);

	QSize tmp1=teachersComboBox->minimumSizeHint();
	Q_UNUSED(tmp1);
	QSize tmp2=studentsComboBox->minimumSizeHint();
	Q_UNUSED(tmp2);
	QSize tmp3=subjectsComboBox->minimumSizeHint();
	Q_UNUSED(tmp3);
	QSize tmp4=activityTagsComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);
	
	updateTeachersComboBox();
	updateStudentsComboBox();
	updateSubjectsComboBox();
	updateActivityTagsComboBox();

	preferredTimesTable->setRowCount(gt.rules.nHoursPerDay);
	preferredTimesTable->setColumnCount(gt.rules.nDaysPerWeek);

	for(int j=0; j<gt.rules.nDaysPerWeek; j++){
		QTableWidgetItem* item=new QTableWidgetItem(gt.rules.daysOfTheWeek[j]);
		preferredTimesTable->setHorizontalHeaderItem(j, item);
	}
	for(int i=0; i<gt.rules.nHoursPerDay; i++){
		QTableWidgetItem* item=new QTableWidgetItem(gt.rules.hoursOfTheDay[i]);
		preferredTimesTable->setVerticalHeaderItem(i, item);
	}

	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			QTableWidgetItem* item=new QTableWidgetItem(YES);
			item->setTextAlignment(Qt::AlignCenter);
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			colorItem(item);
			if(SHOW_TOOLTIPS_FOR_CONSTRAINTS_WITH_TABLES)
				item->setToolTip(gt.rules.daysOfTheWeek[j]+QString("\n")+gt.rules.hoursOfTheDay[i]);
			preferredTimesTable->setItem(i, j, item);
		}
		
	preferredTimesTable->resizeRowsToContents();
	connect(preferredTimesTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(horizontalHeaderClicked(int)));
	connect(preferredTimesTable->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(verticalHeaderClicked(int)));
	
	preferredTimesTable->setSelectionMode(QAbstractItemView::NoSelection);

	setStretchAvailabilityTableNicely(preferredTimesTable);
}

AddConstraintActivitiesPreferredStartingTimesForm::~AddConstraintActivitiesPreferredStartingTimesForm()
{
	saveFETDialogGeometry(this);
}

void AddConstraintActivitiesPreferredStartingTimesForm::colorItem(QTableWidgetItem* item)
{
	if(USE_GUI_COLORS){
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
		if(item->text()==YES)
			item->setBackground(QBrush(QColorConstants::DarkGreen));
		else
			item->setBackground(QBrush(QColorConstants::DarkRed));
		item->setForeground(QBrush(QColorConstants::LightGray));
#else
		if(item->text()==YES)
			item->setBackground(QBrush(Qt::darkGreen));
		else
			item->setBackground(QBrush(Qt::darkRed));
		item->setForeground(QBrush(Qt::lightGray));
#endif
	}
}

void AddConstraintActivitiesPreferredStartingTimesForm::horizontalHeaderClicked(int col)
{
	if(col>=0 && col<gt.rules.nDaysPerWeek){
		QString s=preferredTimesTable->item(0, col)->text();
		if(s==YES)
			s=NO;
		else{
			assert(s==NO);
			s=YES;
		}

		for(int row=0; row<gt.rules.nHoursPerDay; row++){
			/*QString s=notAllowedTimesTable->text(row, col);
			if(s==YES)
				s=NO;
			else{
				assert(s==NO);
				s=YES;
			}*/
			preferredTimesTable->item(row, col)->setText(s);
			colorItem(preferredTimesTable->item(row, col));
		}
	}
}

void AddConstraintActivitiesPreferredStartingTimesForm::verticalHeaderClicked(int row)
{
	if(row>=0 && row<gt.rules.nHoursPerDay){
		QString s=preferredTimesTable->item(row, 0)->text();
		if(s==YES)
			s=NO;
		else{
			assert(s==NO);
			s=YES;
		}
	
		for(int col=0; col<gt.rules.nDaysPerWeek; col++){
			/*QString s=notAllowedTimesTable->text(row, col);
			if(s==YES)
				s=NO;
			else{
				assert(s==NO);
				s=YES;
			}*/
			preferredTimesTable->item(row, col)->setText(s);
			colorItem(preferredTimesTable->item(row, col));
		}
	}
}

void AddConstraintActivitiesPreferredStartingTimesForm::setAllSlotsAllowed()
{
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			preferredTimesTable->item(i, j)->setText(YES);

			colorItem(preferredTimesTable->item(i, j));
		}
}

void AddConstraintActivitiesPreferredStartingTimesForm::setAllSlotsNotAllowed()
{
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			preferredTimesTable->item(i, j)->setText(NO);

			colorItem(preferredTimesTable->item(i, j));
		}
}

void AddConstraintActivitiesPreferredStartingTimesForm::itemClicked(QTableWidgetItem* item)
{
	QString s=item->text();
	
	if(s==YES)
		s=NO;
	else{
		assert(s==NO);
		s=YES;
	}
	item->setText(s);
	colorItem(item);
}

void AddConstraintActivitiesPreferredStartingTimesForm::updateTeachersComboBox(){
	teachersComboBox->clear();
	teachersComboBox->addItem("");
	for(int i=0; i<gt.rules.teachersList.size(); i++){
		Teacher* t=gt.rules.teachersList[i];
		teachersComboBox->addItem(t->name);
	}
}

void AddConstraintActivitiesPreferredStartingTimesForm::updateStudentsComboBox(){
	populateStudentsComboBox(studentsComboBox, QString(""), true);
}

void AddConstraintActivitiesPreferredStartingTimesForm::updateSubjectsComboBox(){
	subjectsComboBox->clear();
	subjectsComboBox->addItem("");
	for(int i=0; i<gt.rules.subjectsList.size(); i++){
		Subject* s=gt.rules.subjectsList[i];
		subjectsComboBox->addItem(s->name);
	}
}

void AddConstraintActivitiesPreferredStartingTimesForm::updateActivityTagsComboBox(){
	activityTagsComboBox->clear();
	activityTagsComboBox->addItem("");
	for(int i=0; i<gt.rules.activityTagsList.size(); i++){
		ActivityTag* s=gt.rules.activityTagsList[i];
		activityTagsComboBox->addItem(s->name);
	}
}

void AddConstraintActivitiesPreferredStartingTimesForm::addConstraint()
{
	TimeConstraint *ctr=nullptr;

	int duration=-1;
	if(durationCheckBox->isChecked()){
		assert(durationSpinBox->isEnabled());
		duration=durationSpinBox->value();
	}

	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage)"));
		return;
	}

	QString teacher=teachersComboBox->currentText();
	if(teacher!="")
		assert(gt.rules.searchTeacher(teacher)>=0);

	QString students=studentsComboBox->currentText();
	if(students!="")
		assert(gt.rules.searchStudentsSet(students)!=nullptr);

	QString subject=subjectsComboBox->currentText();
	if(subject!="")
		assert(gt.rules.searchSubject(subject)>=0);
		
	QString activityTag=activityTagsComboBox->currentText();
	if(activityTag!="")
		assert(gt.rules.searchActivityTag(activityTag)>=0);
		
	if(duration==-1 && teacher=="" && students=="" && subject=="" && activityTag==""){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("You specified all the activities. This might be a small problem: if you specify"
		  " a not allowed slot between two allowed slots, this not allowed slot will"
		  " be counted as a gap in the teachers' and students' timetable.\n\n"
		  " The best practice would be to use constraint break times.\n\n"
		  " If you need weight under 100%, then you can use this constraint, but be careful"
		  " not to obtain an impossible timetable (if your teachers/students are constrained on gaps"
		  " or early gaps and if you leave a not allowed slot between 2 allowed slots or"
		  " a not allowed slot early in the day and more allowed slots after it,"
		  " this possible gap might be counted in teachers' and students' timetable)")
		  +"\n\n"+tr("Do you want to add current constraint?"),
		 QMessageBox::Yes, QMessageBox::Cancel);
		
		if(t==QMessageBox::Cancel)
				return;
	}

	if(duration==-1 && teacher!="" && students=="" && subject=="" && activityTag==""){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("You specified only the teacher. This might be a small problem: if you specify"
		  " a not allowed slot between two allowed slots, this not allowed slot will"
		  " be counted as a gap in the teacher's timetable.\n\n"
		  " The best practice would be to use constraint teacher not available times.\n\n"
		  " If you need weight under 100%, then you can use this constraint, but be careful"
		  " not to obtain an impossible timetable (if your teacher is constrained on gaps"
		  " and if you leave a not allowed slot between 2 allowed slots, this possible"
		  " gap might be counted in teacher's timetable)")
		  +"\n\n"+tr("Do you want to add current constraint?"),
		 QMessageBox::Yes, QMessageBox::Cancel);
		
		if(t==QMessageBox::Cancel)
				return;
	}
	if(duration==-1 && teacher=="" && students!="" && subject=="" && activityTag==""){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("You specified only the students set. This might be a small problem: if you specify"
		  " a not allowed slot between two allowed slots (or a not allowed slot before allowed slots),"
		  " this not allowed slot will"
		  " be counted as a gap (or early gap) in the students' timetable.\n\n"
		  " The best practice would be to use constraint students set not available times.\n\n"
		  " If you need weight under 100%, then you can use this constraint, but be careful"
		  " not to obtain an impossible timetable (if your students set is constrained on gaps or early gaps"
		  " and if you leave a not allowed slot between 2 allowed slots (or a not allowed slot before allowed slots), this possible"
		  " gap might be counted in students' timetable)")
		  +"\n\n"+tr("Do you want to add current constraint?"),
		 QMessageBox::Yes, QMessageBox::Cancel);
		
		if(t==QMessageBox::Cancel)
				return;
	}

	QList<int> days_L;
	QList<int> hours_L;
	int n=0;
	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		for(int i=0; i<gt.rules.nHoursPerDay; i++)
			if(preferredTimesTable->item(i, j)->text()==YES){
				days_L.append(j);
				hours_L.append(i);
				n++;
			}

	if(n<=0){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("Warning: 0 slots selected. Are you sure?"),
		 QMessageBox::Yes, QMessageBox::Cancel);
		
		if(t==QMessageBox::Cancel)
				return;
	}

	ctr=new ConstraintActivitiesPreferredStartingTimes(weight, teacher, students, subject, activityTag, duration, n, days_L, hours_L);

	bool tmp2=gt.rules.addTimeConstraint(ctr);
	if(tmp2){
		QString s=tr("Constraint added:");
		s+="\n\n";
		s+=ctr->getDetailedDescription(gt.rules);
		LongTextMessageBox::information(this, tr("FET information"), s);
	}
	else{
		QMessageBox::warning(this, tr("FET information"),
			tr("Constraint NOT added - duplicate?", "'Duplicate' means that the constraint is identical to an existing constraint and cannot be added."));
		delete ctr;
	}
}

void AddConstraintActivitiesPreferredStartingTimesForm::on_durationCheckBox_toggled()
{
	durationSpinBox->setEnabled(durationCheckBox->isChecked());
}

#undef YES
#undef NO
