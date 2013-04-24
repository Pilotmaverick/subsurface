/*
 * mainwindow.cpp
 *
 * classes for the main UI window in Subsurface
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>
#include <QDateTime>

#include "divelistview.h"
#include "starwidget.h"

#include "glib.h"
#include "../dive.h"
#include "../divelist.h"
#include "../pref.h"


MainWindow::MainWindow() : ui(new Ui::MainWindow()),
			   model(new DiveTripModel(this))
{
	ui->setupUi(this);
	ui->ListWidget->setModel(model);
	setWindowIcon(QIcon(":subsurface-icon"));
	// Just to test the star widgets, can be safely removed.

}

void MainWindow::on_actionNew_triggered()
{
	qDebug("actionNew");
}

void MainWindow::on_actionOpen_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), filter());
	if (filename.isEmpty()) {
		return;
	}

	// Needed to convert to char*
	QByteArray fileNamePtr = filename.toLocal8Bit();

	on_actionClose_triggered();

	GError *error = NULL;
	parse_file(fileNamePtr.data(), &error);
	set_filename(fileNamePtr.data(), TRUE);

	if (error != NULL)
	{
		QMessageBox::warning(this, "Error", error->message);
		g_error_free(error);
		error = NULL;
	}

	//WARNING: Port This method to Qt
	report_dives(FALSE, FALSE);

	ui->InfoWidget->reload();
}

void MainWindow::on_actionSave_triggered()
{
	qDebug("actionSave");
}

void MainWindow::on_actionSaveAs_triggered()
{
	qDebug("actionSaveAs");
}
void MainWindow::on_actionClose_triggered()
{
	if (unsaved_changes() && (askSaveChanges() == FALSE))
	{
		return;
	}

	/* free the dives and trips */
	while (dive_table.nr)
	{
		delete_single_dive(0);
	}
	mark_divelist_changed(FALSE);

	/* clear the selection and the statistics */
	selected_dive = 0;

	//WARNING: Port this to Qt.
	//process_selected_dives();

	ui->InfoWidget->clearStats();
	ui->InfoWidget->clearInfo();
	ui->InfoWidget->clearEquipment();

	clear_events();
	show_dive_stats(NULL);

	/* redraw the screen */
	//WARNING: Port this to Qt.
	dive_list_update_dives();

	// WARNING? Port this to Qt.
	show_dive_info(NULL);
}

void MainWindow::on_actionImport_triggered()
{
	qDebug("actionImport");
}

void MainWindow::on_actionExportUDDF_triggered()
{
	qDebug("actionExportUDDF");
}

void MainWindow::on_actionPrint_triggered()
{
	qDebug("actionPrint");
}

void MainWindow::on_actionPreferences_triggered()
{
	qDebug("actionPreferences");
}

void MainWindow::on_actionQuit_triggered()
{
	qDebug("actionQuit");
}

void MainWindow::on_actionDownloadDC_triggered()
{
	qDebug("actionDownloadDC");
}

void MainWindow::on_actionDownloadWeb_triggered()
{
	qDebug("actionDownloadWeb");}

void MainWindow::on_actionEditDeviceNames_triggered()
{
	qDebug("actionEditDeviceNames");}

void MainWindow::on_actionAddDive_triggered()
{
	qDebug("actionAddDive");
}

void MainWindow::on_actionRenumber_triggered()
{
	qDebug("actionRenumber");
}

void MainWindow::on_actionAutoGroup_triggered()
{
	qDebug("actionAutoGroup");
}

void MainWindow::on_actionToggleZoom_triggered()
{
	qDebug("actionToggleZoom");
}

void MainWindow::on_actionYearlyStatistics_triggered()
{
	qDebug("actionYearlyStatistics");
}

void MainWindow::on_actionViewList_triggered()
{
	qDebug("actionViewList");

	ui->InfoWidget->setVisible(false);
	ui->ListWidget->setVisible(true);
	ui->ProfileWidget->setVisible(false);
}

void MainWindow::on_actionViewProfile_triggered()
{
	qDebug("actionViewProfile");

	ui->InfoWidget->setVisible(false);
	ui->ListWidget->setVisible(false);
	ui->ProfileWidget->setVisible(true);
}

void MainWindow::on_actionViewInfo_triggered()
{
	qDebug("actionViewInfo");

	ui->InfoWidget->setVisible(true);
	ui->ListWidget->setVisible(false);
	ui->ProfileWidget->setVisible(false);
}

void MainWindow::on_actionViewAll_triggered()
{
	qDebug("actionViewAll");

	ui->InfoWidget->setVisible(true);
	ui->ListWidget->setVisible(true);
	ui->ProfileWidget->setVisible(true);
}

void MainWindow::on_actionPreviousDC_triggered()
{
	qDebug("actionPreviousDC");
}

void MainWindow::on_actionNextDC_triggered()
{
	qDebug("actionNextDC");
}

void MainWindow::on_actionSelectEvents_triggered()
{
	qDebug("actionSelectEvents");
}

void MainWindow::on_actionInputPlan_triggered()
{
	qDebug("actionInputPlan");
}

void MainWindow::on_actionAboutSubsurface_triggered()
{
	qDebug("actionAboutSubsurface");
}

void MainWindow::on_actionUserManual_triggered()
{
	qDebug("actionUserManual");
}

QString MainWindow::filter()
{
	QString f;
	f += "ALL ( *.xml *.XML *.uddf *.udcf *.UDFC *.jlb *.JLB ";
#ifdef LIBZIP
	f += "*.sde *.SDE *.dld *.DLD ";
#endif
#ifdef SQLITE3
	f += "*.db";
#endif
	f += ");;";

	f += "XML (*.xml *.XML);;";
	f += "UDDF (*.uddf);;";
	f += "UDCF (*.udcf *.UDCF);;";
	f += "JLB  (*.jlb *.JLB);;";

#ifdef LIBZIP
	f += "SDE (*.sde *.SDE);;";
	f += "DLD (*.dld *.DLD);;";
#endif
#ifdef SQLITE3
	f += "DB (*.db)";
#endif

	return f;
}

bool MainWindow::askSaveChanges()
{
	QString message = ! existing_filename ? tr("You have unsaved changes\nWould you like to save those before closing the datafile?")
		:    tr("You have unsaved changes to file: %1 \nWould you like to save those before closing the datafile?").arg(existing_filename);

	if (QMessageBox::question(this,  tr("Save Changes?"), message) == QMessageBox::Ok) {
		// WARNING: Port.
		//		file_save(NULL,NULL);
		return true;
	}
	return false;
}
