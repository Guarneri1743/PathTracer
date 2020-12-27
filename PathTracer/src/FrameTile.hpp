#ifndef _FRAME_TILE_HPP_
#define _FRAME_TILE_HPP_

namespace Guarneri
{
	typedef struct
	{
		uint32_t tile_size;
		uint32_t tile_task_size;
		uint32_t row_tile_count;
		uint32_t col_tile_count;
		uint32_t tile_length;
	} TileInfo;

	struct FrameTile
	{
	public:
		uint32_t tile_idx;
		uint32_t row_start;
		uint32_t row_end;
		uint32_t col_start;
		uint32_t col_end;
		bool shaded;

	public:
		FrameTile();
		~FrameTile();
		static int coord2index(const int& row, const int& col, const int& col_tile_count);
		static void build_tiles(
			FrameTile* tiles,
			const int& tile_size,
			const int& row_tile_count, const int& col_tile_count,
			const int& row_rest, const int& col_rest);
	};

	FrameTile::FrameTile()
	{
		tile_idx = 0;
		row_start = 0;
		row_end = 0;
		col_start = 0;
		col_end = 0;
		shaded = false;
	}

	FrameTile::~FrameTile()
	{

	}

	int FrameTile::coord2index(
		const int& row,
		const int& col,
		const int& col_tile_count)
	{
		return row * col_tile_count + col;
	}

	void FrameTile::build_tiles(
		FrameTile* tiles,
		const int& tile_size,
		const int& row_tile_count, const int& col_tile_count,
		const int& row_rest, const int& col_rest)
	{
		for (int row = 0; row < row_tile_count; row++)
		{
			for (int col = 0; col < col_tile_count; col++)
			{
				bool last = false;
				int tidx = coord2index(row, col, col_tile_count);
				int rs = row * tile_size;
				tiles[tidx].row_start = rs;
				last = row == row_tile_count - 1 ? true : false;
				tiles[tidx].row_end = last && row_rest > 0 ? rs + row_rest : (row + 1) * tile_size;

				int cs = col * tile_size;
				tiles[tidx].col_start = cs;
				last = col == col_tile_count - 1 ? true : false;
				tiles[tidx].col_end = last && col_rest > 0 ? cs + col_rest : (col + 1) * tile_size;

				tiles[tidx].tile_idx = tidx;
			}
		}
	}
}
#endif