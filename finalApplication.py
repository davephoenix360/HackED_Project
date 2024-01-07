import openrouteservice as ors
import folium
#from functools import reduce
#import operator
import googlemaps
import api_keys

GOOGLE_API_KEY = api_keys.GOOGLE_API_KEY
google_client = googlemaps.Client(GOOGLE_API_KEY)
ORS_API_KEY = api_keys.ORS_API_KEY
ors_client = ors.Client(ORS_API_KEY)

m = folium.Map(location=list((53.5260682, -113.520915)), zoom_start=13)#, tiles="cartodbpositron")

def get_Geocode(address, city="Edmonton", region="Alberta", country="Canada"):
    
    res = google_client.geocode(address + city + region + country)
    return res[0]['geometry']['location']

def get_directions(origin, destination):
    
    origin_coords = get_Geocode(origin)
    dest_coords = get_Geocode(destination)
    
    coords = [[origin_coords['lng'], origin_coords['lat']],[dest_coords['lng'], dest_coords['lat']]] # (long, lat)
    route = google_client.directions(origin, destination, mode= 'walking', avoid=["highways", "tolls", "ferries"], units="metric", region="us")
    route = ors_client.directions(coordinates=coords, profile='foot-walking', format='geojson')
    
    
    #print(route[0]['legs'])
    
    #print(route['features'][0]['properties']['segments'][0]['steps'])
    
    #waypoints = list(dict.fromkeys(reduce(operator.concat ,list(map(lambda step: step['way_points'], route['features'][0]['properties']['segments'][0]['steps'])))))

    #folium.PolyLine(locations=[list(reversed(coord)) for coord in route['features'][0]['geometry']['coordinates']], color='blue').add_to(m)

    #folium.PolyLine(locations=[list(reversed(route['features'][0]['geometry']['coordinates'][index])) for index in waypoints], color='red').add_to(m)

    #m.show_in_browser()

    return route['features'][0]['properties']['segments'][0]['steps']

def simplify_drections(directions):
    simplified = ""
    for direction in directions:
        simplified += direction['instruction'] + " and move forward for " + str(direction['distance'])
        if directions.index(direction) != len(directions)-1:
            simplified += ", "
        else:
            simplified += '.'
    
    return simplified

#print(get_Geocode("9005 112 ST NW", "Edmonton", "Alberta", "Canada"))
#my_directions = simplify_drections(get_directions("9005 112 ST NW", "9120 116 St NW T6G 2V4"))
#print(get_Geocode("9005 112 ST NW"))
