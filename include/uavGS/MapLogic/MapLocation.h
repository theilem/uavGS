#ifndef LOCATION_H
#define LOCATION_H

#include <QJsonObject>
#include <cpsCore/Utilities/LinearAlgebra.h>
#include <cpsCore/Configuration/Configuration.hpp>
#include <cpsCore/Configuration/Parameter.hpp>

struct MapLocationParams
{
	Parameter<double> lat = {0.0, "lat", true};
	Parameter<double> lon = {0.0, "lon", true};

	template<typename Config>
	inline void
	configure(Config& c)
	{
		c & lat;
		c & lon;
	}
};

class MapLocation
{
public:
	explicit MapLocation(double east = 0, double north = 0);
	explicit MapLocation(const MapLocationParams& params);

	static MapLocation
	fromJson(const Configuration& json);

	static MapLocation
	fromLatLong(double Lat, double Long);

	static MapLocation
	fromMapTileCoords(double x, double y, int z);

	static MapLocation
	fromUTM(double northing, double easting);

	static MapLocation
	fromVector3(Vector3 vect, int zone = 0, char hemi = 'N');

	double
	northing() const;

	double
	easting() const;

	double
	x(int z) const;

	double
	y(int z) const;

	double
	latitude() const;

	double
	longitude() const;

	int
	getZone() const;

	char
	getHemi() const;

	void
	setZone(int);

	void
	setHemi(char);

private:
	double n_;
	double e_;
	int zone_;
	char hemi_;

	double lat_;
	double lon_;

	static void
	latLongToUTM(double Lat, double Long, double* northing, double* easting, int* zone,
				 char* hemi);

	static void
	mapTileCoordsToLatLong(double x, double y, int z, double* Lat, double* Long);
};

#endif // LOCATION_H
