/***************************************************************************
                          addconstraintstudentssetnotavailabletimesform.cpp  -  description
                             -------------------
    begin                : Feb 10, 2005
    copyright            : (C) 2005 by Lalescu Liviu
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

#include "addconstraintstudentssetnotavailabletimesform.h"
#include "timeconstraint.h"

#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QBrush>
#include <QColor>

#define YES		(QString("X"))
#define NO		(QString(" "))

AddConstraintStudentsSetNotAvailableTimesForm::AddConstraintStudentsSetNotAvailableTimesForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	addConstraintPushButton->setDefault(true);

	connect(addConstraintPushButton, SIGNAL(clicked()), this, SLOT(addCurrentConstraint()));
	connect(closePushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(notAllowedTimesTable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(itemClicked(QTableWidgetItem*)));
	connect(helpPushButton, SIGNAL(clicked()), this, SLOT(help()));
	connect(setAllAvailablePushButton, SIGNAL(clicked()), this, SLOT(setAllAvailable()));
	connect(setAllNotAvailablePushButton, SIGNAL(clicked()), this, SLOT(setAllNotAvailable()));

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);

	QSize tmp2=studentsComboBox->minimumSizeHint();
	Q_UNUSED(tmp2);
	
	updateStudentsSetComboBox();

	notAllowedTimesTable->setRowCount(gt.rules.nHoursPerDay);
	notAllowedTimesTable->setColumnCount(gt.rules.nDaysPerWeek);

	for(int j=0; j<gt.rules.nDaysPerWeek; j++){
		QTableWidgetItem* item=new QTableWidgetItem(gt.rules.daysOfTheWeek[j]);
		notAllowedTimesTable->setHorizontalHeaderItem(j, item);
	}
	for(int i=0; i<gt.rules.nHoursPerDay; i++){
		QTableWidgetItem* item=new QTableWidgetItem(gt.rules.hoursOfTheDay[i]);
		notAllowedTimesTable->setVerticalHeaderItem(i, item);
	}

	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			QTableWidgetItem* item=new QTableWidgetItem(NO);
			item->setTextAlignment(Qt::AlignCenter);
			item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
			colorItem(item);
			if(SHOW_TOOLTIPS_FOR_CONSTRAINTS_WITH_TABLES)
				item->setToolTip(gt.rules.daysOfTheWeek[j]+QString("\n")+gt.rules.hoursOfTheDay[i]);
			notAllowedTimesTable->setItem(i, j, item);
		}
		
	notAllowedTimesTable->resizeRowsToContents();

	connect(notAllowedTimesTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(horizontalHeaderClicked(int)));
	connect(notAllowedTimesTable->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(verticalHeaderClicked(int)));

	notAllowedTimesTable->setSelectionMode(QAbstractItemView::NoSelection);

	setStretchAvailabilityTableNicely(notAllowedTimesTable);
}

AddConstraintStudentsSetNotAvailableTimesForm::~AddConstraintStudentsSetNotAvailableTimesForm()
{
	saveFETDialogGeometry(this);
}

void AddConstraintStudentsSetNotAvailableTimesForm::colorItem(QTableWidgetItem* item)
{
	if(USE_GUI_COLORS){
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
		if(item->text()==NO)
			item->setBackground(QBrush(QColorConstants::DarkGreen));
		else
			item->setBackground(QBrush(QColorConstants::DarkRed));
		item->setForeground(QBrush(QColorConstants::LightGray));
#else
		if(item->text()==NO)
			item->setBackground(QBrush(Qt::darkGreen));
		else
			item->setBackground(QBrush(Qt::darkRed));
		item->setForeground(QBrush(Qt::lightGray));
#endif
	}
}

void AddConstraintStudentsSetNotAvailableTimesForm::horizontalHeaderClicked(int col)
{
	if(col>=0 && col<gt.rules.nDaysPerWeek){
		QString s=notAllowedTimesTable->item(0, col)->text();
		if(s==YES)
			s=NO;
		else{
			assert(s==NO);
			s=YES;
		}

		for(int row=0; row<gt.rules.nHoursPerDay; row++){
			notAllowedTimesTable->item(row, col)->setText(s);
			colorItem(notAllowedTimesTable->item(row,col));
		}
	}
}

void AddConstraintStudentsSetNotAvailableTimesForm::verticalHeaderClicked(int row)
{
	if(row>=0 && row<gt.rules.nHoursPerDay){
		QString s=notAllowedTimesTable->item(row, 0)->text();
		if(s==YES)
			s=NO;
		else{
			assert(s==NO);
			s=YES;
		}
	
		for(int col=0; col<gt.rules.nDaysPerWeek; col++){
			notAllowedTimesTable->item(row, col)->setText(s);
			colorItem(notAllowedTimesTable->item(row,col));
		}
	}
}

void AddConstraintStudentsSetNotAvailableTimesForm::setAllAvailable()
{
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			notAllowedTimesTable->item(i, j)->setText(NO);
			colorItem(notAllowedTimesTable->item(i,j));
		}
}

void AddConstraintStudentsSetNotAvailableTimesForm::setAllNotAvailable()
{
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			notAllowedTimesTable->item(i, j)->setText(YES);
			colorItem(notAllowedTimesTable->item(i,j));
		}
}

void AddConstraintStudentsSetNotAvailableTimesForm::updateStudentsSetComboBox()
{
	populateStudentsComboBox(studentsComboBox);
}

void AddConstraintStudentsSetNotAvailableTimesForm::itemClicked(QTableWidgetItem* item)
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

void AddConstraintStudentsSetNotAvailableTimesForm::addCurrentConstraint()
{
	TimeConstraint *ctr=nullptr;

	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<100.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage). It has to be 100"));
		return;
	}

	QString students_name=studentsComboBox->currentText();
	StudentsSet* s=gt.rules.searchStudentsSet(students_name);
	if(s==nullptr){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid students set"));
		return;
	}

	QList<int> days;
	QList<int> hours;
	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		for(int i=0; i<gt.rules.nHoursPerDay; i++)
			if(notAllowedTimesTable->item(i, j)->text()==YES){
				days.append(j);
				hours.append(i);
			}

	ctr=new ConstraintStudentsSetNotAvailableTimes(weight, students_name, days, hours);

	bool tmp2=gt.rules.addTimeConstraint(ctr);
	if(tmp2)
		LongTextMessageBox::information(this, tr("FET information"),
			tr("Constraint added:")+"\n\n"+ctr->getDetailedDescription(gt.rules));
	else{
		QMessageBox::warning(this, tr("FET information"),
			tr("Constraint NOT added - there must be another constraint of this "
			  "type referring to the same students set. Please edit that one"));
		delete ctr;
	}
}

void AddConstraintStudentsSetNotAvailableTimesForm::help()
{
	QString s;
	
	s=tr("This constraint does not induce gaps (or early gaps) for students. If a student set has lessons"
	 " before and after a not available period, gaps will not be counted.");
	
	s+="\n\n";
	
	s+=tr("If you really need to use weight under 100%, you can use activities preferred times with"
	 " only the students set specified, but this might generate problems, as possible gaps will be"
	 " counted and you may obtain an impossible timetable");

	QMessageBox::information(this, tr("FET help"), s);
}
