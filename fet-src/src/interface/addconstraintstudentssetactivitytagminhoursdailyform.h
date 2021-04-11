/***************************************************************************
                          addconstraintstudentssetactivitytagminhoursdailyform.h  -  description
                             -------------------
    begin                : 2019
    copyright            : (C) 2019 by Lalescu Liviu
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

#ifndef ADDCONSTRAINTSTUDENTSSETACTIVITYTAGMINHOURSDAILYFORM_H
#define ADDCONSTRAINTSTUDENTSSETACTIVITYTAGMINHOURSDAILYFORM_H

#include "ui_addconstraintstudentssetactivitytagminhoursdailyform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintStudentsSetActivityTagMinHoursDailyForm : public QDialog, Ui::AddConstraintStudentsSetActivityTagMinHoursDailyForm_template  {
	Q_OBJECT
public:
	AddConstraintStudentsSetActivityTagMinHoursDailyForm(QWidget* parent);
	~AddConstraintStudentsSetActivityTagMinHoursDailyForm();
	
	void updateStudentsSetComboBox();
	void updateActivityTagsComboBox();

public slots:
	void addCurrentConstraint();
};

#endif
