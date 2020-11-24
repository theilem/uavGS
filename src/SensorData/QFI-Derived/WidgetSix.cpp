/***************************************************************************//**
 * @file example/WidgetSix.cpp
 * @author  Marek M. Cel <marekcel@mscsim.org>
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 Marek M. Cel
 *
 * This file is part of QFlightInstruments. You can redistribute and modify it
 * under the terms of GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Further information about the GNU General Public License can also be found
 * on the world wide web at http://www.gnu.org.
 *
 * ---
 *
 * Copyright (C) 2013 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/
#ifndef WIDGETSIX_CPP
#define WIDGETSIX_CPP
#endif

////////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "uavGS/SensorData/QFlightInstruments/WidgetSix.h"
#include "ui_WidgetSix.h"
#include <string>
////////////////////////////////////////////////////////////////////////////////

WidgetSix::WidgetSix(QWidget* parent) :
		QWidget(parent), m_ui(new Ui::WidgetSix),

		m_widgetADI(0), m_widgetALT(0), m_widgetASI(0), m_widgetHSI(0), m_widgetTC(0), m_widgetVSI(
				0)
{
	m_ui->setupUi(this);

	m_widgetADI = m_ui->widgetADI;
	m_widgetALT = m_ui->widgetALT;
	m_widgetASI = m_ui->widgetASI;
	m_widgetHSI = m_ui->widgetHSI;
	m_widgetTC = m_ui->widgetTC;
	m_widgetVSI = m_ui->widgetVSI;
}

////////////////////////////////////////////////////////////////////////////////

WidgetSix::~WidgetSix()
{
	if (m_ui)
	{
		delete m_ui;
		m_ui = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////

void
WidgetSix::update()
{
	m_ui->widgetADI->update();
	m_ui->widgetALT->update();
	m_ui->widgetASI->update();
	m_ui->widgetHSI->update();
	m_ui->widgetTC->update();
	m_ui->widgetVSI->update();
}


void
WidgetSix::contentUpdatedSlot()
{
	m_ui->widgetADI->setRoll(sensorData_.attitude.x() * 180. / M_PI);
	m_ui->widgetADI->setPitch(sensorData_.attitude.y() * 180. / M_PI);
	m_ui->widgetALT->setAltitude(sensorData_.position.z() * 3.28084); //convert from ENU in meters to altitude in feet
	//m_ui->widgetASI->setAirspeed(sensorData_.velocityAir*1.94384); //convert from m/s to knots TODO fixme
	m_ui->widgetHSI->setHeading(sensorData_.attitude.z() * 180. / M_PI);
	m_ui->widgetTC->setTurnRate(sensorData_.angularRate.z() * 180. / M_PI);
	m_ui->widgetVSI->setClimbRate(sensorData_.velocity.z() * 19.685); //convert from m/s to 100ft/min
	this->update();
}

void
WidgetSix::connect()
{
	QObject::connect(this, SIGNAL(contentUpdated()), this, SLOT(contentUpdatedSlot()));
	if (auto sdm = get<SensorDataManager>())
	{
		sdm->subscribeOnSensorDataLocal([this](const auto& sd)
										{ sensorData_ = sd; emit contentUpdated(); });
	}
}
