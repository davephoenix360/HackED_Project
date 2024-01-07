import openrouteservice as ors
import folium
from functools import reduce
import api_keys
import operator

API_KEY = api_keys.ORS_API_KEY
client = ors.Client(API_KEY)

# Note Folium works with (lat, long) while ors works with (long, lat) which is why we used reverse

m = folium.Map(location=list(reversed([-77.0362619, 38.897475])), tiles="cartodbpositron", zoom_start=13)

# white house to pentagon
coords = [[-77.0362619, 38.897475], [-77.0584556, 38.871861]]  # (long, lat)

route = client.directions(coordinates=coords,
                          profile='driving-car',
                          format='geojson')

waypoints = list(dict.fromkeys(reduce(operator.concat ,list(map(lambda step: step['way_points'], route['features'][0]['properties']['segments'][0]['steps'])))))

folium.PolyLine(locations=[list(reversed(coord)) for coord in route['features'][0]['geometry']['coordinates']], color='blue').add_to(m)

folium.PolyLine(locations=[list(reversed(route['features'][0]['geometry']['coordinates'][index])) for index in waypoints], color='red').add_to(m)

m.show_in_browser()
