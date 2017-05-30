function Date=GpsToDate(Week,Sow)
% GPSTODATE - convert GPS week no. and second of week to matlab datenum
% Note that this function only converts the timeformat, it does not convert 
% between GPS time and UTC. Also note that matlab datenum is not accurate
% to mm precision, but datevec or Week and Sow are.
%
% eps(now)*3e8 > 1e-3
%
% input
%   Week          - GPS week number
%   Sow           - GPS second of week
%
% output
%   Date          - Matlab datenumber
%
% created
%   Apr 2014      - P.F. de Bakker (TU Delft)
%
% See also DATETOGPS, GPSTOVEC, VECTOGPS

% datenum([1980,1,6])=723186;
Date=(7*Week+Sow/(24*3600))+723186;
