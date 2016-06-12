/*****************************************************************************
* Copyright 2015-2016 Alexander Barthel albar965@mailbox.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef MAPQUERY_H
#define MAPQUERY_H

#include "geo/pos.h"
#include "geo/rect.h"
#include "geo/linestring.h"
#include "common/maptypes.h"
#include "maplayer.h"

#include <QCache>
#include <QList>

#include <marble/GeoDataCoordinates.h>
#include <marble/GeoDataLatLonBox.h>

namespace atools {
namespace geo {
class Rect;
}
namespace sql {
class SqlDatabase;
class SqlQuery;
}
}

class CoordinateConverter;
class MapTypesFactory;

class MapQuery
  : public QObject
{
  Q_OBJECT

public:
  MapQuery(QObject *parent, atools::sql::SqlDatabase *sqlDb);
  ~MapQuery();

  void getAirportAdminById(int airportId, QString& city, QString& state, QString& country);

  void getAirportById(maptypes::MapAirport& airport, int airportId);

  void getAirwaysForWaypoint(QList<maptypes::MapAirway>& airways, int waypointId);

  void getAirwayById(maptypes::MapAirway& airway, int airwayId);

  void getVorForWaypoint(maptypes::MapVor& vor, int waypointId);

  void getNdbForWaypoint(maptypes::MapNdb& ndb, int waypointId);

  void getMapObjectByIdent(maptypes::MapSearchResult& result, maptypes::MapObjectTypes type,
                           const QString& ident, const QString& region = QString());

  void getMapObjectById(maptypes::MapSearchResult& result, maptypes::MapObjectTypes type, int id);

  void getNearestObjects(const CoordinateConverter& conv, const MapLayer *mapLayer, bool airportDiagram,
                         maptypes::MapObjectTypes types, int xs, int ys, int screenDistance,
                         maptypes::MapSearchResult& result);

  void getParkingByNameAndNumber(QList<maptypes::MapParking>& parkings, int airportId, const QString& name,
                                 int number);

  const QList<maptypes::MapAirport> *getAirports(const Marble::GeoDataLatLonBox& rect,
                                                 const MapLayer *mapLayer, bool lazy);

  const QList<maptypes::MapWaypoint> *getWaypoints(const Marble::GeoDataLatLonBox& rect,
                                                   const MapLayer *mapLayer, bool lazy);

  const QList<maptypes::MapVor> *getVors(const Marble::GeoDataLatLonBox& rect, const MapLayer *mapLayer,
                                         bool lazy);

  const QList<maptypes::MapNdb> *getNdbs(const Marble::GeoDataLatLonBox& rect, const MapLayer *mapLayer,
                                         bool lazy);

  const QList<maptypes::MapMarker> *getMarkers(const Marble::GeoDataLatLonBox& rect, const MapLayer *mapLayer,
                                               bool lazy);

  const QList<maptypes::MapIls> *getIls(const Marble::GeoDataLatLonBox& rect, const MapLayer *mapLayer,
                                        bool lazy);

  const QList<maptypes::MapAirway> *getAirways(const Marble::GeoDataLatLonBox& rect, const MapLayer *mapLayer,
                                               bool lazy);

  const QList<maptypes::MapRunway> *getRunwaysForOverview(int airportId);

  const QList<maptypes::MapRunway> *getRunways(int airportId);

  const QList<maptypes::MapApron> *getAprons(int airportId);

  const QList<maptypes::MapTaxiPath> *getTaxiPaths(int airportId);

  const QList<maptypes::MapParking> *getParkingsForAirport(int airportId);

  const QList<maptypes::MapHelipad> *getHelipads(int airportId);

  atools::geo::Rect getAirportRect(int airportId);

  void initQueries();
  void deInitQueries();

  atools::sql::SqlDatabase *getDatabase() const
  {
    return db;
  }

signals:
  void resultTruncated(maptypes::MapObjectTypes type, int truncatedTo);

private:
  template<typename TYPE>
  struct SimpleCache
  {
    bool handleCache(const Marble::GeoDataLatLonBox& rect, const MapLayer *mapLayer, bool lazy);
    void clear();

    Marble::GeoDataLatLonBox curRect;
    const MapLayer *curMapLayer = nullptr;
    QList<TYPE> list;
  };

  MapTypesFactory *mapTypesFactory;
  atools::sql::SqlDatabase *db;

  SimpleCache<maptypes::MapAirport> airportCache;
  SimpleCache<maptypes::MapWaypoint> waypointCache;
  SimpleCache<maptypes::MapVor> vorCache;
  SimpleCache<maptypes::MapNdb> ndbCache;
  SimpleCache<maptypes::MapMarker> markerCache;
  SimpleCache<maptypes::MapIls> ilsCache;
  SimpleCache<maptypes::MapAirway> airwayCache;

  QCache<int, QList<maptypes::MapRunway> > runwayCache;
  QCache<int, QList<maptypes::MapRunway> > runwayOverwiewCache;
  QCache<int, QList<maptypes::MapApron> > apronCache;
  QCache<int, QList<maptypes::MapTaxiPath> > taxipathCache;
  QCache<int, QList<maptypes::MapParking> > parkingCache;
  QCache<int, QList<maptypes::MapHelipad> > helipadCache;

  atools::sql::SqlQuery *airportByRectQuery = nullptr, *airportMediumByRectQuery = nullptr,
  *airportLargeByRectQuery = nullptr;

  atools::sql::SqlQuery *runwayOverviewQuery = nullptr, *apronQuery = nullptr,
  *parkingQuery = nullptr, *helipadQuery = nullptr, *taxiparthQuery = nullptr, *runwaysQuery = nullptr,
  *parkingTypeAndNumberQuery = nullptr;

  atools::sql::SqlQuery *waypointsByRectQuery = nullptr, *vorsByRectQuery = nullptr,
  *ndbsByRectQuery = nullptr, *markersByRectQuery = nullptr, *ilsByRectQuery = nullptr,
  *airwayByRectQuery = nullptr;

  atools::sql::SqlQuery *airportByIdentQuery = nullptr, *vorByIdentQuery = nullptr,
  *ndbByIdentQuery = nullptr, *waypointByIdentQuery = nullptr;

  atools::sql::SqlQuery *vorByIdQuery = nullptr, *ndbByIdQuery = nullptr,
  *vorByWaypointIdQuery = nullptr, *ndbByWaypointIdQuery = nullptr, *waypointByIdQuery = nullptr;

  atools::sql::SqlQuery *airportByIdQuery = nullptr, *airportAdminByIdQuery = nullptr;
  atools::sql::SqlQuery *airwayByWaypointIdQuery = nullptr;
  atools::sql::SqlQuery *airwayByIdQuery = nullptr;

  const QList<maptypes::MapAirport> *fetchAirports(const Marble::GeoDataLatLonBox& rect,
                                                   atools::sql::SqlQuery *query, bool reverse,
                                                   bool lazy, bool overview);

  void bindCoordinatePointInRect(const Marble::GeoDataLatLonBox& rect, atools::sql::SqlQuery *query,
                                 const QString& prefix = QString());

  QList<Marble::GeoDataLatLonBox> splitAtAntiMeridian(const Marble::GeoDataLatLonBox& rect);

  static void inflateRect(Marble::GeoDataLatLonBox& rect, double width, double height);

  bool runwayCompare(const maptypes::MapRunway& r1, const maptypes::MapRunway& r2);

  template<typename TYPE>
  void checkOverflow(const QList<TYPE>& list, maptypes::MapObjectTypes type);

};

// ---------------------------------------------------------------------------------
namespace  {
const double RECT_INFLATION_FACTOR = 0.3;
const double RECT_INFLATION_ADD = 0.1;
const int QUERY_ROW_LIMIT = 3000;
}

template<typename TYPE>
bool MapQuery::SimpleCache<TYPE>::handleCache(const Marble::GeoDataLatLonBox& rect, const MapLayer *mapLayer,
                                              bool lazy)
{
  if(lazy)
    return false;

  Marble::GeoDataLatLonBox cur(curRect);
  MapQuery::inflateRect(cur, cur.width(Marble::GeoDataCoordinates::Degree) *
                        RECT_INFLATION_FACTOR + RECT_INFLATION_ADD,
                        cur.height(Marble::GeoDataCoordinates::Degree) *
                        RECT_INFLATION_FACTOR + RECT_INFLATION_ADD);

  if(curRect.isEmpty() || !cur.contains(rect) ||
     !curMapLayer->hasSameQueryParameters(mapLayer))
  {
    list.clear();
    curRect = rect;
    curMapLayer = mapLayer;
    return true;
  }
  return false;
}

template<typename TYPE>
void MapQuery::SimpleCache<TYPE>::clear()
{
  list.clear();
  curRect.clear();
  curMapLayer = nullptr;
}

#endif // MAPQUERY_H
