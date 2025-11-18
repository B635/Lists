#include "model/book.h"
#include <google/protobuf/timestamp.pb.h>

static std::chrono::system_clock::time_point
timestampToTimePoint(const google::protobuf::Timestamp& ts) {
    using namespace std::chrono;
    auto secs  = system_clock::time_point{seconds(ts.seconds())};
    auto nanos = duration_cast<system_clock::duration>(nanoseconds(ts.nanos()));
    return secs + nanos;
}

static google::protobuf::Timestamp
timePointToTimestamp(const std::chrono::system_clock::time_point& tp) {
    using namespace std::chrono;
    google::protobuf::Timestamp ts;
    auto dur = tp.time_since_epoch();
    auto secs = duration_cast<seconds>(dur);
    auto nanos = duration_cast<nanoseconds>(dur - secs);
    ts.set_seconds(secs.count());
    ts.set_nanos(static_cast<int32_t>(nanos.count()));
    return ts;
}

Book fromProto(const lists::book::Book& pb) {
    Book b;

    b.id = pb.id();
    b.title = pb.title();

    if (!pb.cover_image_url().empty()) {
        b.cover_image_url = pb.cover_image_url();
    }

    if (!pb.author().empty()) {
        b.author = pb.author();
    }

    if (!pb.note().empty()) {
        b.note = pb.note();
    }

    if (pb.has_startdate()) {
        b.start_date = timestampToTimePoint(pb.startdate());
    }
    
    if (pb.has_enddate()) {
        b.end_date = timestampToTimePoint(pb.enddate());
    }

    if (pb.rating() != lists::book::RATING_UNSPECIFIED) {
        b.rating = static_cast<int>(pb.rating());
    }

    return b;
}

lists::book::Book toProto(const Book& b) {
    lists::book::Book pb;
    
    pb.set_id(b.id);
    pb.set_title(b.title);

    if (b.cover_image_url) {
        pb.set_cover_image_url(*b.cover_image_url);
    }
    if (b.author) {
        pb.set_author(*b.author);
    }
    if (b.note) {
        pb.set_note(*b.note);
    }

    if (b.start_date) {
        *pb.mutable_startdate() = timePointToTimestamp(*b.start_date);
    }

    if (b.end_date) {
        *pb.mutable_enddate() = timePointToTimestamp(*b.end_date);
    }

    if (b.rating) {
        pb.set_rating(static_cast<lists::book::Rating>(*b.rating));
    } else {
        pb.set_rating(lists::book::RATING_UNSPECIFIED);
    }

    return pb;
}

Book makeBookFromCreateRequest(const lists::book::api::CreateBookRequest& req,
                                      int64_t new_id) {
    Book b;
    b.id    = new_id;
    b.title = req.title();  

    if (!req.author().empty()) {
        b.author = req.author();
    }
    if (!req.cover_image_url().empty()) {
        b.cover_image_url = req.cover_image_url();
    }
    if (!req.note().empty()) {
        b.note = req.note();
    }

    if (req.has_start_date()) {
        b.start_date = timestampToTimePoint(req.start_date());
    }
    if (req.has_end_date()) {
        b.end_date = timestampToTimePoint(req.end_date());
    }

    // rating：枚举 0 代表未设置
    if (req.rating() != lists::book::RATING_UNSPECIFIED) {
        b.rating = static_cast<int>(req.rating());
    }

    return b;
}